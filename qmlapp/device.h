#ifndef THEVFC_H
#define THEVFC_H

#include <QObject>
#include <QProcess>
#include <QTimer>

#include "modulebase.h"
#include "qdevicewatcher.h"
#include "deviceprofile.h"
#include "itrobj.h"

//#include <nzmqt/nzmqt.hpp>
//using namespace nzmqt;

class QTranslator;

class TheDevice;


class TheDevice : public ModuleBase, public ITrObj
{
    Q_OBJECT

    Q_ENUMS(DriveDirection)
    Q_ENUMS(DriveMode)

public:

    enum DriveDirection {
        DD_UNDEF             = -1,
        DD_DOWN              = 0, //DRIVE_DOWN,
        DD_UP                = 1 //DRIVE_UP
    };

    enum DriveMode {
        DM_UNDEF = -1,
        DM_STOP  = 0,//DRIVE_STOP,
        DM_ACC   = 1,//DRIVE_ACC,
        DM_DEC   = 2,//DRIVE_DECL,
        DM_REF   = 3,//DRIVE_REF
    };

    static TheDevice *instance();

    typedef bool (*parsefunc)(int, const QJsonArray&);

    QHash<int, parsefunc> parseFunctions;

    static bool parseVer0(int version, const QJsonArray& data);

    TheDevice();
    ~TheDevice();

    void updateTranslation();

    Q_PROPERTY(bool isVFCConnected READ vfcConnected NOTIFY vfcConnectedChanged)
    Q_PROPERTY(bool isGatewayConnected READ gatewayConnected NOTIFY gatewayConnectedChanged)
    Q_PROPERTY(bool statusWord READ statusWord NOTIFY statusWordChanged)
    Q_PROPERTY(bool miningStarted READ miningStarted NOTIFY miningStartedChanged)
    Q_PROPERTY(bool usbFlashConnected READ usbFlashConnected NOTIFY usbFlashConnectedChanged)
    Q_PROPERTY(bool sdConnected READ sdConnected NOTIFY sdConnectedChanged)
    Q_PROPERTY(QString headerText READ headerText NOTIFY headerTextChanged)
    Q_PROPERTY(QString serialNumber READ SMH4SerialNumber NOTIFY serialNumberChanged)

    Q_PROPERTY(QVariant Ia READ Ia NOTIFY IaChanged)
    Q_PROPERTY(QVariant Ip READ Ip NOTIFY IpChanged)
    Q_PROPERTY(QVariant PCmm READ PCmm NOTIFY PCmmChanged)
    Q_PROPERTY(QVariant Vrpm READ Vrpm NOTIFY VrpmChanged)
    Q_PROPERTY(QVariant Fkg READ Fkg NOTIFY FkgChanged)
    Q_PROPERTY(QVariant trusted READ trusted NOTIFY trustedChanged)
    Q_PROPERTY(QVariant detector READ detector NOTIFY detectorChanged)

    Q_PROPERTY(DriveDirection driveDirection READ driveDirection WRITE setDriveDirection NOTIFY driveDirectionChanged)
    Q_PROPERTY(DriveMode driveMode READ driveMode WRITE setDriveMode NOTIFY driveModeChanged)
    Q_PROPERTY(int speedMode READ speedMode WRITE setSpeedMode NOTIFY speedModeChanged)

    Q_PROPERTY(double Ns READ Ns WRITE setNs NOTIFY NsChanged) // скорость привода в текущий момент
    Q_PROPERTY(double P_N1 READ P_N1 NOTIFY P_N1Changed) //
    Q_PROPERTY(double P_N2u READ P_N2u NOTIFY P_N2uChanged)
    Q_PROPERTY(double P_N2d READ P_N2d NOTIFY P_N2dChanged)
    Q_PROPERTY(double P_Nomax READ P_Nomax NOTIFY P_NomaxChanged)
    Q_PROPERTY(double P_Nomin READ P_Nomin NOTIFY P_NominChanged)


    Q_INVOKABLE void powerOnInit();
    Q_INVOKABLE void resetFaults();
    Q_INVOKABLE void startMining();
    Q_INVOKABLE void setMiningSpeed(double speed);
    Q_INVOKABLE void stopMining();

    Q_INVOKABLE QString speedmodeToString(int speedMode);

    double speed() const;
    void setSpeed(double speed);

    uint statusWord() const;
    void setStatusWord(const uint &statusWord);

    double NoC() const;
    void setNoC(double NoC);

    bool vfcConnected() const;
    bool gatewayConnected() const;
    bool miningStarted() const;

    bool usbFlashConnected() const;
    void setUsbFlashConnected(bool usbFlashConnected);


    bool sdConnected() const;
    void setSdConnected(bool sdConnected);

    QString headerText() const;
    void updateHeaderText();

    void setHeaderText(const QString &headerText);

    void parseProfileRawData32(uint32_t P_DATA32);
    void resetProfile();
    QString profileToString();


    void setRlID(int value);
    void setGrID(int value);
    void setMtrID(int value);
    void setBrID(int value);
    void setRdID(int value);
    void setBrrID(int value);
    void setVfcID(int value);

    int rlID() const;

    QVariant Ia() const;
    void setIa(QVariant Ia);

    QVariant Ip() const;
    void setIp(const QVariant &Ip);

    QVariant PCmm() const;
    void setPCmm(QVariant PCmm);

    QVariant Vrpm() const;
    void setVrpm(QVariant Vrpm);

    QVariant Fkg() const;
    void setF(QVariant Fkg);

    QVariant trusted() const;
    void setTrusted(bool trusted);

    QVariant detector() const;
    void setDetector(QVariant detector);

    //Matheng *engine() const;

    void execRTmodel(bool writeOutput);

//    ZMQContext *zmqctx() const;

    void setMiningStarted(bool miningStarted);
    QString SMH4SerialNumber();

    double P_N1() const;
    void setP_N1(double P_N1);

    double P_N2u() const;
    void setP_N2u(double P_N2u);

    double P_N2d() const;
    void setP_N2d(double P_N2d);

    double P_Nomax() const;
    void setP_Nomax(double P_Nomax);

    double P_Nomin() const;
    void setP_Nomin(double P_Nomin);

    void processData0(
            double Ia,
            double PCmm,
            double Vrpm,
            double F,
            bool trusted,
            bool detector
         );

    DriveDirection driveDirection() const;
    void setDriveDirection(const DriveDirection &driveDir);

    DriveMode driveMode() const;
    void setDriveMode(const DriveMode &driveMode);

    double Ns() const;
    void setNs(double Ns);

    int speedMode() const;
    void setSpeedMode(int speedMode);

public slots:
    void onAir(DataObject *data);
    
    void onVFConnectionChanged();
    void onGatewayConnectionChanged();
    void onGUIUpdateTimer();

    void onStream1Data(
            double Ia,
            double PCmm,
            double Vrpm,
            double F,
            bool trusted,
            bool detector
         );


    void slotDeviceAdded(const QString& dev);
    void slotDeviceRemoved(const QString& dev);
    void slotDeviceChanged(const QString& dev);

    void execMountProc();
    void execUSBFlashUMountProc();
    void onMountReadyReadStandardOutput();

    void onDumperDone(int exitCode);
    void onUMounerDone(int exitCode);

    void onProfileData(int rl, int gr,int mtr, int br, int rd, int brr, int vfc);

    void zsend(const QByteArray &msg);

    void parseData(const QJsonArray& ja);
    void onZMQMessageReceived(const QList<QByteArray>& msg);
    void ondPCmmTimer();
signals:
    void transmit(DataObject *data);

    void vfcConnectedChanged();
    void gatewayConnectedChanged();
    void statusWordChanged();
    void miningStartedChanged();

//    void usbFlashMountStatusChanged(bool connected);
    void usbFlashConnectedChanged();
    void sdConnectedChanged();
    void headerTextChanged();

    void profileDataLoaded(int rl, int gr,int mtr, int br, int rd, int brr, int vfc);

    void IaChanged();
    void IpChanged();
    void PCmmChanged();
    void VrpmChanged();
    void FkgChanged();
    void trustedChanged();
    void detectorChanged();

    void send2FC(QByteArray data);
    void serialNumberChanged();

    void stream1parsed(
            double Ia,
            double PCmm,
            double Vrpm,
            double F,
            bool trusted,
            bool detector
    );

    void P_N1Changed();
    void P_N2uChanged();
    void P_N2dChanged();
    void P_NomaxChanged();
    void P_NominChanged();

    void driveDirectionChanged();
    void driveModeChanged();
    void NsChanged();
    void speedModeChanged();

protected:

private:
    void setVFCconnected(bool vfcConnected);
    void setGatewayConnected(bool gatewayConnected);

private:
    static TheDevice* s_instance;
    double _speed;
    uint _statusWord;
    double _NoC;

    bool _vfcConnected;
    bool _gatewayConnected;

    QDeviceWatcher *_devWatcher;
    QProcess *_mountChecker;
    QProcess *_umounter;
    bool _usbFlashConnected = false;
    bool _sdConnected = false;
    bool _removeUSBFlash = false;
    QProcess *_dumper;
    QString _headerText;
    DeviceProfileDB *_dpdb = nullptr;

//    DevProfile _devProfile;

    int _rlID = -1;
    int _grID = -1;
    int _mtrID = -1;
    int _brID = -1;
    int _rdID = -1;
    int _brrID = -1;
    int _vfcID = -1;

    QVariant _Ia;
    QVariant _Ip;
    QVariant _PCmm;
    QVariant _Vrpm;
    QVariant _F;
    QVariant _trusted;
    QVariant _detector;

    bool _cfIa = true;
    bool _cfPCmm = true;
    bool _cfVrpm = true;
    bool _cfSF = true;
    bool _cftrusted = true;
    bool _cfdetector = true;

    //Matheng *_engine;
//    ZMQContext *_ctx;
//    ZMQSocket *_zsockPub;
//    ZMQSocket *_zsockSub;

    bool _miningStarted = true;


    QTimer *_dPCmmTimer;
    double _PCmmOld = 0.0;
    QString _SN;

    double _current_V;//текущая скорость в оборотах/мин
    double _current_N;//текущая скорость в ходах/мин

    double _P_N1 = 1.0;
    double _P_N2u = 3.0;
    double _P_N2d = 2.0;
    double _P_Nomax = 5.0;
    double _P_Nomin = 1.0;

    DriveDirection _driveDir = DD_UNDEF;
    DriveMode _driveMode = DM_UNDEF;

    double _Ns = 0.0;
    int _speedMode = 0;



};

#define DEV TheDevice::instance()

#endif // THEVFC_H
