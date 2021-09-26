#include "device.h"

#include <QDebug>

#include <QtMath>
#include <QMutex>
#include <QGuiApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QJsonDocument>

#include <math.h>


#include "air.h"
#include "eventsmodel.h"
#include "vfccommproto.h"
#include "logger.h"
#include "ntgpiotools.h"
#include "utils.h"

#include "deviceprofile.h"
#include "_Proto.h"
#include "version.h"

//#include "matheng.h"
//#include "lrprtmodel.h"


#define MOUNT_CHK_TIMEOUT_MSEC 1000

TheDevice* TheDevice::s_instance = nullptr;

TheDevice *TheDevice::instance()
{

    if (s_instance) return s_instance;

    if (s_instance == nullptr) {
        s_instance = new TheDevice;
    }

    return s_instance;
}

bool TheDevice::parseVer0(int version, const QJsonArray &ja)
{
//    qDebug() << "parseVer0" << version << dumpJOA(ja);
    int xchg_ver = ja[0].toInt();
    if (xchg_ver != 0) return false;

    QJsonObject jdata = ja[1].toObject();

    //qDebug() << "parseVer0" << dumpJO(jdata);

    //setDetector( QVariant::fromValue();
    double Ia = jdata["D_Ia"].toArray().at(0).toDouble();
    double Ip = jdata["D_I"].toArray().at(0).toDouble();
    double PCmm = jdata["D_PC"].toArray().at(0).toDouble();
    double Vrpm = jdata["D_V"].toArray().at(0).toDouble();
    double F =  jdata["D_F"].toArray().at(0).toDouble();
    bool trusted = false;
    bool detector = (jdata["D_Detect"].toArray().at(0).toInt()) == 1;
    double Ns =  jdata["D_N"].toArray().at(0).toDouble();

    DriveDirection dd = (DriveDirection)jdata["D_DrvDir"].toArray().at(0).toInt();
    DriveMode dm = (DriveMode)jdata["D_DrvStat"].toArray().at(0).toInt();


    TheDevice::instance()->setDetector( detector );
    TheDevice::instance()->setIa(Ia);
    TheDevice::instance()->setIp(Ip);
    TheDevice::instance()->setPCmm(PCmm);
    TheDevice::instance()->setVrpm(Vrpm);
    TheDevice::instance()->setNs(Ns);

    TheDevice::instance()->setDriveDirection(dd);
    TheDevice::instance()->setDriveMode(dm);

    //TheDevice::instance()->setVrpm( jdata["D_V"].toArray().at(0).toDouble() );


    DEV->processData0(
        Ia,
        PCmm,
        Vrpm,
        F,
        trusted,
        detector
    );


    return true;
}

TheDevice::TheDevice()
    : ModuleBase(nullptr)
    , _speed(450.0)
    , _statusWord(0)
    , _NoC(41.9) //Номинальный ток мотора, А <-- рефактор
    , _vfcConnected(false)
    , _gatewayConnected(false)
    , _devWatcher(new QDeviceWatcher(this))
    , _dpdb(new DeviceProfileDB)
    //, _engine(new Matheng)
//    , _ctx(createDefaultContext(this))
//    , _zsockPub(nullptr)
//    , _zsockSub(nullptr)
{
    if (s_instance == nullptr) {
        s_instance = this;
    } else {
        mLog() << "attempt to create second TheDevice::TheDevice";
    }

    //initialize serialNumber
    SMH4SerialNumber();

    connect(_devWatcher, SIGNAL(deviceAdded(QString)), this, SLOT(slotDeviceAdded(QString)));
    connect(_devWatcher, SIGNAL(deviceChanged(QString)), this, SLOT(slotDeviceChanged(QString)));
    connect(_devWatcher, SIGNAL(deviceRemoved(QString)), this, SLOT(slotDeviceRemoved(QString)));
    _devWatcher->start();

    _mountChecker = new QProcess(this);
    _umounter = new QProcess(this);
    _dumper = new QProcess(this);

    QTimer::singleShot(MOUNT_CHK_TIMEOUT_MSEC, this, SLOT(execMountProc()));

    connect(_mountChecker, SIGNAL(readyReadStandardOutput()), this, SLOT(onMountReadyReadStandardOutput()));
    connect(_umounter, SIGNAL(finished(int)), this, SLOT(onUMounerDone(int)));
    connect(_dumper, SIGNAL(finished(int)), this, SLOT(onDumperDone(int)));

//    connect(this, SIGNAL(usbFlashMountStatusChanged(bool)), this, SLOT(onUSBFlashMountStatusChanged(bool)));

    linkWithAir<TheDevice>(this);
    mLog() << QString("TheDevice profile %1").arg(profileToString());

    updateHeaderText();

    connect(this, SIGNAL(profileDataLoaded(int,int,int,int,int,int,int)), this, SLOT(onProfileData(int,int,int,int,int,int,int)));

    QTimer *guiUpdater = new QTimer(this);
    connect(guiUpdater, &QTimer::timeout, this, &TheDevice::onGUIUpdateTimer);

    guiUpdater->start(500);

//    _ctx = createDefaultContext(this);
//    _ctx->start();
//    _zsockPub = _ctx->createSocket(ZMQSocket::TYP_PUB, this);
//    _zsockPub->bindTo("tcp://*:1234");

//    _zsockSub = _ctx->createSocket(ZMQSocket::TYP_SUB, this);
//    connect(_zsockSub, &ZMQSocket::messageReceived, this, &TheDevice::onZMQMessageReceived);
//    _zsockSub->setObjectName("Subscriber.Socket.socket(SUB)");
//    _zsockSub->subscribeTo("ping");

//    _zsockSub->connectTo("tcp://127.0.0.1:4321");
////    _zsockSub->connectTo("tcp://192.168.0.213:4321");
////    _zsockSub->connectTo("tcp://192.168.0.100:4321");

    _dPCmmTimer = new QTimer(this);

    _dPCmmTimer->setInterval(1000);
    _dPCmmTimer->start();
    connect(_dPCmmTimer, &QTimer::timeout, this, &TheDevice::ondPCmmTimer);

    connect(this, &TheDevice::stream1parsed, this, &TheDevice::onStream1Data);

    //register parse function
    parseFunctions.insert(0, &TheDevice::parseVer0);

    //setSpeedMode(0);
}

double TheDevice::NoC() const
{
    return _NoC;
}

void TheDevice::setNoC(double NoC)
{
    _NoC = NoC;
}

bool TheDevice::vfcConnected() const
{
    return true;// _vfcConnected;
}

bool TheDevice::gatewayConnected() const
{
    return true;//_gatewayConnected;
}

bool TheDevice::miningStarted() const
{

    return _miningStarted;
}
/*
void TheVFC::dump2UserInfo_ConnectionStatus()
{
    if (FCCommunicator::instance()->vfcConnectStatus()) {
        SMSG_S(tr("<font color=\"#00660a\"><b>Связь с ПЧ установлена</b><font color=\"#109f10\">"));
    } else {
        SMSG_F(tr("<font color=\"#9f1010\">Связь с ПЧ потеряна</font>"));
    }
}
*/
uint TheDevice::statusWord() const
{
    return _statusWord;
}

void TheDevice::setStatusWord(const uint &statusWord)
{
    if (_statusWord == statusWord) return;
    qDebug() << "TheDevice::setStatusWord" << statusWord;
    _statusWord = statusWord;

    emit statusWordChanged();
}

double TheDevice::speed() const
{
    return _speed;
}

void TheDevice::setSpeed(double speed)
{
    _speed = speed;
}

/*
QTranslator *TheDeviceWrapper::t_en() const
{
    return _t_en;
}

QTranslator *TheDeviceWrapper::t_ru() const
{
    return _t_ru;
}
*/



TheDevice::~TheDevice()
{
    delete _dpdb;
    //delete _engine;
    qDebug() << "TheDevice::~TheDevice done";
}

void TheDevice::updateTranslation()
{
    updateHeaderText();
}

void TheDevice::powerOnInit()
{
    qDebug() << "TheDeviceWrapper::powerOnInit";

//    SMSG_I(tr("съешь ещё этих мягких французских булок да выпей же чаю %1 ").arg(QDateTime::currentDateTime().toString()));

    SMSG_I(tr("<font color=\"#005609\">ПЛК запущен. ПО версия <b><font style=\"font-family:Oxygen Mono\">%1</font></b> <small><font style=\"font-family:Oxygen Mono\">#%2</font></small>").arg(VersionInfo::VERSION).arg(VersionInfo::GIT_COMMIT_ID));

    //dump2UserInfo_ConnectionStatus();
    //qApp->installTranslator(_t_en);
}

void TheDevice::resetFaults()
{
    SMSG_W(tr("Сброс ошибок"))
    qDebug() << "TheDevice::resetFaults()";

}

void TheDevice::startMining()
{
    resetFaults();
    qDebug() << "TheDevice::startMining()";

    SMSG_W(tr("Команда Запуск"))
    QJsonObject jo{
        {"C_Run", 1},

        {"P_N1", P_N1()},

        {"P_N2u", P_N2u()},
        {"P_N2d", P_N2d()},

        {"P_Nomax", P_Nomax()},
        {"P_Nomin", P_Nomin()},
    };
    QJsonDocument jDoc(jo);
    TheDevice::instance()->zsend(jDoc.toJson(QJsonDocument::Compact));


}

void TheDevice::setMiningSpeed(double speed)
{
    qDebug() << "TheDevice::setMiningSpeed" << speed;

}

void TheDevice::stopMining()
{
    qDebug() << "TheDevice::stopMining()";
    QJsonObject jo{
        {"C_Run", 0}
    };
    QJsonDocument jDoc(jo);
    TheDevice::instance()->zsend(jDoc.toJson(QJsonDocument::Compact));
}

QString TheDevice::speedmodeToString(int speedmode)
{
    switch (speedmode) {

    case 0: {
        return tr("Оптимизирующий");
    }
    case 1: {
        return tr("1-скоростной");

    } break;
    case 2: {
        return tr("2-скоростной");

    } break;

    default: {
        return QString();
    }
    }
}

void TheDevice::onAir(DataObject *data)
{
    qDebug() << "TheDeviceWrapper::onAir" << data;
}

void TheDevice::onMountReadyReadStandardOutput()
{
    QProcess *proc = dynamic_cast<QProcess*>(sender());

    if (proc) {
        QStringList sl(QString(proc->readAll()).split(QChar('\n')) );
        bool ufc = false;
        bool sdc = false;
        foreach (QString s, sl) {
            if (s.contains("/media/u1 type vfat")) {
                ufc = true;
            }

            if (s.contains("/dev/mmcblk1p1 on /media/sd1 type vfat")) {
                qDebug() << "parse mount:" << s;
                sdc = true;
            }
        }

        setUsbFlashConnected(ufc);
        setSdConnected(sdc);
    }
}

void TheDevice::onVFConnectionChanged()
{
//    qDebug() << "!!!TheDevice::onVFConnectionChanged()" << FCCommunicator::instance()->vfcConnectStatus() << FCCommunicator::instance()->gatewayConnectStatus();
//    setVFCconnected(FCCommunicator::instance()->vfcConnectStatus());

    if (vfcConnected() ) {
        SMSG_S(tr("<font color=\"#00660a\">Связь с ПЧ установлена<font color=\"#109f10\">"));
        NTGPIOTools::instance()->turnOffLed1();
    } else {
        SMSG_F(tr("<font color=\"#9f1010\"><b>Связь с ПЧ потеряна</b></font>"));
        NTGPIOTools::instance()->turnOnLed1();
    }

//updateHeaderText();
//    emit headerTextChanged();

}

void TheDevice::onGatewayConnectionChanged()
{
//    setGatewayConnected(FCCommunicator::instance()->gatewayConnectStatus());

//    emit headerTextChanged();

}

void TheDevice::onGUIUpdateTimer()
{
    if (_cfdetector) {
        emit detectorChanged();
        _cfdetector = false;
    }

    if (_cftrusted) {
        emit trustedChanged();
        _cftrusted = false;
    }

    if (_cfPCmm) {
        emit PCmmChanged();
        _cfPCmm = false;
    }

    if (_cfSF) {
        emit FkgChanged();
        _cfSF = false;
    }

    if (_cfVrpm) {
        emit VrpmChanged();
        _cfVrpm = false;
    }

    if (_cfIa) {
        emit IaChanged();
        _cfIa = false;
    }

}

void TheDevice::onStream1Data(double Ia, double PCmm, double Vrpm, double F, bool trusted, bool detector)
{
   //qDebug() << Ia << PCmm << Vrpm << SLkg;

    setIa(Ia);
    setPCmm(PCmm);
    setVrpm(Vrpm);
    setF(F);
    setTrusted(trusted);
    setDetector(detector);
}

void TheDevice::slotDeviceAdded(const QString &dev)
{
    qDebug() << "TheDevice::slotDeviceAdded" << dev;
    if (dev.contains("/dev/sd")) {
        QTimer::singleShot(MOUNT_CHK_TIMEOUT_MSEC, this, SLOT(execMountProc()));
    }

    if (dev.contains("/dev/mmcblk1p1")) {
       setSdConnected(true);
    }
}

void TheDevice::slotDeviceRemoved(const QString &dev)
{
    qDebug() << "TheDevice::slotDeviceRemoved" << dev;
    if (dev.contains("/dev/sd")) {
        _removeUSBFlash = false;
        setUsbFlashConnected(false);
    }
    if (dev.contains("/dev/mmcblk1p1")) {
       setSdConnected(false);
    }
}

void TheDevice::slotDeviceChanged(const QString &dev)
{
    Q_UNUSED(dev)
}

void TheDevice::execMountProc()
{
    if (_mountChecker->processId() == 0)
        _mountChecker->start("mount");
}

void TheDevice::execUSBFlashUMountProc()
{
    _umounter->start("umount", QStringList() << "/media/u1");
}

void TheDevice::onDumperDone(int exitCode)
{
    mLog() << QString("ThePump::onDumperDone %1").arg(exitCode);
    if (exitCode == 0) {
        SMSG_I(tr("Сброс архива завершён"));
        execUSBFlashUMountProc();
    } else {
        SMSG_F(QString("Ошибка дампера %1").arg(exitCode));
        _removeUSBFlash = true;
    }
//    invalidateMenu();

}

void TheDevice::onUMounerDone(int exitCode)
{
    mLog() << QString("onUMounerDone %1").arg(exitCode);
    _removeUSBFlash = true;
    if (exitCode == 0) {
        SMSG_I(tr("Отсоедините USB-Flash"));
    } else {
        SMSG_F(tr("Ошибка отключения USB-Flash %1").arg(exitCode));
    }
    //    invalidateMenu();

}

void TheDevice::updateHeaderText()
{
    const QString singleArrowUp = "<font style=\"font-family:Symbol\">&#xad;</font>";
    const QString singleArrowDown = "<font style=\"font-family:Symbol\">&#xaf;</font>";
    const QString doubleArrowUp = "<font style=\"font-family:Symbol\">&#xdd;</font>";
    const QString doubleArrowDown = "<font style=\"font-family:Symbol\">&#xde;</font>";

    QString ht;

    bool vfcConnected = true;//FCCommunicator::instance()->vfcConnectStatus();
    bool gatewayConnected = true;// FCCommunicator::instance()->gatewayConnectStatus();


    //&#xad;&#xaf;
    if (vfcConnected && gatewayConnected) {
//        ht = mningStarted() ? tr("<b><font style=\"font-family:Oxygen Mono\"><font style=\"font-family:Symbol\">&#xdd;</font>5.7</font></b> х/мин <small><font style=\"font-family:Symbol\">&#xad;</font>2.3<font style=\"font-family:Symbol\">&#xaf;</font>4.5</small>") : (tr("<font style=\"font-family:Roboto\">Ожидает запуска</font><font style=\"font-family:Oxygen Mono\">%1</font>").arg(SMH4SerialNumber()));
        QString currArr;
        if (driveDirection() == DD_UP) {
            currArr = singleArrowUp;
        }if (driveDirection() == DD_DOWN) {
            currArr = singleArrowDown;
        }
//!
//!        ht = miningStarted() ? tr("<b><font style=\"font-family:Oxygen Mono\">%1%2</font></b>х/мин <small><font style=\"font-family:Symbol\">&#xad;</font>2.3<font style=\"font-family:Symbol\">&#xaf;</font>4.5</small>").arg(currArr).arg(Ns()) : (tr("<font style=\"font-family:Roboto\">Ожидает запуска</font>"));
        ht = miningStarted() ? tr("Запущен <b><font style=\"font-family:Oxygen Mono\">%1%2</font></b>х/мин <small>").arg(currArr).arg(Ns()) : tr("<font style=\"font-family:Roboto\">Ожидает запуска</font>");
    } else {
        ht = tr("<b>Нет связи с ПЧ</b>") + (gatewayConnected ? tr(". Проверь линию ШЛЮЗ &lt;-&gt; ПЧ. ") : "");
    }

    setHeaderText(ht);
}

void TheDevice::setVFCconnected(bool vfcConnected)
{
    if (_vfcConnected == vfcConnected) return;

    qDebug() << "TheVFC::setVFCconnected" << vfcConnected;
    _vfcConnected = vfcConnected;

    emit vfcConnectedChanged(); updateHeaderText();
}

void TheDevice::setGatewayConnected(bool gatewayConnected)
{
    if (_gatewayConnected == gatewayConnected) return;

    qDebug() << "TheVFC::setGatewayConnected" << gatewayConnected;
    _gatewayConnected = gatewayConnected;
    emit gatewayConnectedChanged();

    updateHeaderText();
}

int TheDevice::speedMode() const
{
    return _speedMode;
}

void TheDevice::setSpeedMode(int speedMode)
{
    qDebug() << "TheDevice::setSpeedMode" << speedMode;
    if (_speedMode == speedMode) return;
    _speedMode = speedMode;
    emit speedModeChanged();
}

double TheDevice::Ns() const
{
    return _Ns;
}

void TheDevice::setNs(double Ns)
{
    if (cmpFV(_Ns, Ns)) return;
    _Ns = Ns;

    updateHeaderText();
    emit NsChanged();
}


void TheDevice::setDriveDirection(const DriveDirection &driveDir)
{
    if (_driveDir == driveDir) return;
    _driveDir = driveDir;

    updateHeaderText();
    emit driveDirectionChanged();
}

void TheDevice::setDriveMode(const DriveMode &driveMode)
{
    if (_driveMode == driveMode) return;
    _driveMode = driveMode;

    updateHeaderText();
    emit driveModeChanged();
}


TheDevice::DriveDirection TheDevice::driveDirection() const
{
    return _driveDir;
}

TheDevice::DriveMode TheDevice::driveMode() const
{
    return  _driveMode;
}

double TheDevice::P_Nomin() const
{
    return _P_Nomin;
}

void TheDevice::setP_Nomin(double P_Nomin)
{
    _P_Nomin = P_Nomin;
}

void TheDevice::processData0(double Ia, double PCmm, double Vrpm, double F, bool trusted, bool detector)
{
    emit stream1parsed(Ia, PCmm, Vrpm, F, trusted, detector);
}

double TheDevice::P_Nomax() const
{
    return _P_Nomax;
}

void TheDevice::setP_Nomax(double P_Nomax)
{
    _P_Nomax = P_Nomax;
}

double TheDevice::P_N2d() const
{
    return _P_N2d;
}

void TheDevice::setP_N2d(double P_N2d)
{
    _P_N2d = P_N2d;
}

double TheDevice::P_N2u() const
{
    return _P_N2u;
}

void TheDevice::setP_N2u(double P_N2u)
{
    _P_N2u = P_N2u;
}

double TheDevice::P_N1() const
{
    return _P_N1;
}

void TheDevice::setP_N1(double P_N1)
{
    _P_N1 = P_N1;
}


void TheDevice::setMiningStarted(bool miningStarted)
{
    if (_miningStarted == miningStarted) return;

    _miningStarted = miningStarted;

    emit miningStartedChanged();
    updateHeaderText();
}

//ZMQContext *TheDevice::zmqctx() const
//{
//    return _ctx;
//}



void TheDevice::setDetector(QVariant detector)
{
    if (_detector == detector) return;
    _detector = detector;
    _cfdetector = true;
    //    emit detectorChanged();
}


QVariant TheDevice::trusted() const
{
    return _trusted;
}

void TheDevice::setTrusted(bool trusted)
{
    if (_trusted == trusted) return;
    _trusted = trusted;
    _cftrusted = true;
//    emit trustedChanged();
}

QVariant TheDevice::detector() const
{
    return _detector;
}

QVariant TheDevice::Fkg() const
{
    return _F;
}

void TheDevice::setF(QVariant F)
{
    if (cmpFV(_F, F)) return;
    _F = F;
    _cfSF = true;
//    emit SLkgChanged();
}

QVariant TheDevice::Vrpm() const
{
//    return _Vrpm;
    return QString::number(_Vrpm.toDouble(), 'f', 0);
}

void TheDevice::setVrpm(QVariant Vrpm)
{
    if (cmpFV(_Vrpm, Vrpm)) return;
    _Vrpm = Vrpm;
    _cfVrpm  = true;
//    emit VrpmChanged();
}

QVariant TheDevice::PCmm() const
{
    //return _PCmm;
    return QString::number(_PCmm.toDouble(), 'f', 0);
}

void TheDevice::setPCmm(QVariant PCmm)
{
    if (cmpFV(_PCmm, PCmm)) return;
    _PCmm = PCmm;
    _cfPCmm = true;
//    emit PCmmChanged();
}

QVariant TheDevice::Ia() const
{
    return QString::number(_Ia.toDouble(), 'f', 1);
}

void TheDevice::setIa(QVariant Ia)
{
    if (cmpFV(_Ia, Ia)) return;
    _Ia = Ia;
    _cfIa = true;
//    emit IaChanged();
}


QVariant TheDevice::Ip() const
{
    return QString::number(_Ip.toDouble(), 'f', 1);
}

void TheDevice::setIp(const QVariant &Ip)
{
    if (cmpFV(_Ip, Ip)) return;
    _Ip = Ip;
    emit IpChanged();
}

int TheDevice::rlID() const
{
    return _rlID;
}

void TheDevice::setHeaderText(const QString &headerText)
{
    _headerText = headerText;
    emit headerTextChanged();
}

void TheDevice::parseProfileRawData32(uint32_t P_DATA32)
{
    int rl = ((P_DATA32 >> P_VAL_RLs32) & P_VAL_RL);     // Профиль рейки
    int gr = ((P_DATA32 >> P_VAL_GRs32) & P_VAL_GR);     //
    int mtr = ((P_DATA32 >> P_VAL_MTRs32 ) & P_VAL_MTR);

    int br = ((P_DATA32 >> P_VAL_BRs) & P_VAL_BR);
    int rd = ((P_DATA32 >> P_VAL_RDs) & P_VAL_RD);
    int brr = ((P_DATA32 >> P_VAL_BRRs) & P_VAL_BRR);
    int vfc = (P_DATA32 & P_VAL_VFC);

//    applyParsedProfileData(rl, gr, mtr, br, rd, vfc);

    emit profileDataLoaded(rl, gr, mtr, br, rd, brr, vfc);
}

void TheDevice::onProfileData(int rl, int gr, int mtr, int br, int rd, int brr, int vfc)
{
    //qDebug() << "onProfileData" << rl << gr << mtr << br << rd << brr << vfc;


    setRlID(rl);
    setGrID(gr);
    setMtrID(mtr);
    setBrID(br);
    setRdID(rd);
    setBrrID(brr);
    setVfcID(vfc);

//    SMSG_I(tr("Сброс архива завершён"));

}

void TheDevice::zsend(const QByteArray &msg)
{
//    if (!_zsockPub) return;

    //qDebug() << "zsend" << msg.toHex(' ');
    QList<QByteArray> sl;
    sl << QByteArray("ping");
    sl << msg;
//    _zsockPub->sendMessage(sl);
}

void TheDevice::parseData(const QJsonArray &ja)
{
    int xchg_ver = ja[0].toInt();
    //qDebug() << "parseData" << "xchg_ver" << xchg_ver;

    parseFunctions[xchg_ver](xchg_ver, ja);
}


void TheDevice::onZMQMessageReceived(const QList<QByteArray> &msg)
{
    //qDebug() << "TheDevice::onMessageReceived" << msg;
    //QString s = "onZMQ: ";

    QJsonDocument jDoc = QJsonDocument::fromJson(msg.at(1));


    if (jDoc.isArray()) {
        QJsonArray ja = jDoc.array();
//        s += dumpJOA(ja);

        parseData(ja);

    } else if (jDoc.isObject()) {
        QJsonObject jo = jDoc.object();
//         s += dumpJO(jo);

    }


//    qDebug() << s;
}

void TheDevice::ondPCmmTimer()
{
    double PCmm = _PCmm.toDouble();
    double dPCmm = PCmm - _PCmmOld;
    setMiningStarted(!cmpF(dPCmm, 0));
    _PCmmOld = PCmm;
}

void TheDevice::resetProfile()
{

}

QString TheDevice::profileToString() {
       return QString("%1 %2 %3 %4 %5 %6 %7")
               .arg(_rlID)
               .arg(_grID)
               .arg(_mtrID)
               .arg(_brID)
               .arg(_rdID)
               .arg(_brrID)
               .arg(_vfcID);
}

QString TheDevice::headerText() const
{
    //text: (device.isVFCConnected ? ((device.miningStarted ? qsTr("ЛПШН запущен <b>8.7</b> х/мин") : (qsTr("ЛПШН остановлен")))+ tswitch.lang)
    //                                  : qsTr("<b>Нет связи с ПЧ</b>") + ((device.isGatewayConnected && !device.isVfcConnected )? ". Проверь линию ШЛЮЗ &lt;-&gt; ПЧ. " : ""))

    return _headerText;

//    qDebug() << "TheDevice::headerText vfcConnected()" << vfcConnected();

//    if (vfcConnected()) {
//        return miningStarted() ? tr("ЛПШН запущен <b>8.7</b> х/мин") : (tr("ЛПШН остановлен"));
//    } else {
//        return tr("<b>Нет связи с ПЧ</b>") + ((gatewayConnected() && !vfcConnected()) ? tr(". Проверь линию ШЛЮЗ &lt;-&gt; ПЧ. ") : "");
//    }
}

bool TheDevice::sdConnected() const
{
    return _sdConnected;
}

void TheDevice::setSdConnected(bool sdConnected)
{
    //
    qDebug() << "TheDevice::setSdConnected" << sdConnected;
    if (_sdConnected == sdConnected) return;
    _sdConnected = sdConnected;

    emit sdConnectedChanged();
}

void TheDevice::setUsbFlashConnected(bool usbFlashConnected)
{
    qDebug() << "TheDevice::setUsbFlashConnected" << usbFlashConnected;
    if (_usbFlashConnected == usbFlashConnected) return;

    _usbFlashConnected = usbFlashConnected;
    emit usbFlashConnectedChanged();
}

bool TheDevice::usbFlashConnected() const
{
    return _usbFlashConnected;
}

void TheDevice::setGrID(int value)
{

    if (value == 0) {
        return;
    }

    if (_grID == value) {
        return;
    }

    _grID = value;

    mLog() << QString("Gear %1").arg(DeviceProfileDB::s_GearById.value(value));
}

void TheDevice::setMtrID(int value)
{
    if (value == 0) {
        return;
    }
    if (_mtrID == value) {
        return;
    }
    _mtrID = value;

    mLog() << (QString("Motor id:%1 %2").arg(value).arg(DeviceProfileDB::s_motorById.value(value).toString()));
}

void TheDevice::setBrID(int value)
{
    if (value == 0) {
        return;
    }
    if (_brID == value) {
        return;
    }
    _brID = value;

    mLog() << ( QString("Break %1").arg(value == 3 ? "EXISTS" : "Not found"));
}

void TheDevice::setRdID(int value)
{
    if (value == 0) {
        return;
    }
    if (_rdID == value) {
        return;
    }
    _rdID = value;
    mLog() << QString("Reducer id:%1 %2").arg(value).arg(DeviceProfileDB::s_reducerById.value(value).toString());
}

void TheDevice::setBrrID(int value)
{
    if (value == 0) {
        return;
    }
    if (_brrID == value) {
        return;
    }
    _brrID = value;
    mLog() << (QString("Brake resistor %1").arg(value));

}

void TheDevice::setVfcID(int value)
{
    if (value == 0) {
        return;
    }
    if (_vfcID == value) {
        return;
    }

    _vfcID = value;
    mLog() << (QString("VFC id:%1 %2").arg(value).arg(DeviceProfileDB::s_VFCById.value(value).toString()));
}

void TheDevice::setRlID(int value)
{
    if (value == 0) {
        return;
    }
    if (_rlID == value) {
        return;
    }

    _rlID = value;
    mLog() << (QString("Rail id:%1 %2").arg(value).arg(DeviceProfileDB::s_railById.value(value).toString()));
}

QString TheDevice::SMH4SerialNumber()
{
    if (!_SN.isEmpty()) {
        return _SN;
    }

    QFile f("/proc/sv");

    QString serial("host");
    if (f.open(QIODevice::ReadOnly)) {

        QByteArray fc(f.readAll());
        f.close();

        QString fs(fc);
        QStringList lines = fs.split('\n');

        foreach(QString s, lines) {
            QStringList spl(s.split(QChar(':')));
            if (spl.count() == 2) {
                if (spl.at(0) == "SN") {
                    serial = spl.at(1).trimmed();
                    break;
                }
            }
        }
    }



    _SN  = serial;
    emit serialNumberChanged();
    return serial;
}
