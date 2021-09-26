#include "thegui.h"

#include <QDebug>

#include <QGuiApplication>
#include <QThread>
#include <QtConcurrent>
#include <QTimer>
#include <QTranslator>

#include "menuitem.h"
#include "dynamicmenutypes.h"
#include "customtype.h"
#include "thedevice.h"
#include "translateswitcher.h"
#include "eventsmodel.h"
#include "logger.h"
#include "mathematics.h"
#include "utils.h"

#include "matheng.h"

TheGUI* TheGUI::g_gui = nullptr;

QVariant TheGUI::getGraphLims(MenuItem *miv, const QString &name)
{
    double d = MATH->engine()->getVar(name);
    qDebug() << "TheGUI::getGraphLims" << name << d;
    return d;
}

void TheGUI::setGraphLims(MenuItem *miv, const QString &name, const QVariant &value)
{
    qDebug() << "TheGUI::setGraphLims" << name << value;
    return MATH->engine()->setVar(name, value.toDouble());
}

TheGUI* TheGUI::instance()
{
    return g_gui;
}

#define TRT(x) [=](MenuItem *obj) { \
obj->setText(x); \
}

#define TRU(x) [](MenuItemValue *obj) { \
obj->setUnits(x); \
}

TheGUI::TheGUI(QObject *parent)
    : QObject(parent)
    , _isGuiDark(false)
    , _rootMenu(new Menu(TRT(tr("Главное меню")), 0))
    , _currentMenu(_rootMenu)
    , _tswitch()
    , _events(new EventsModel(this))
{
    g_gui = this;
    createMenu();
    connect(&_tswitch, SIGNAL(languageChanged()), this, SLOT(onLangChanged()));

    QTimer *t = new QTimer(this);
    t->setInterval(5000);

    QTimer *t2 = new QTimer(this);
    t2->setInterval(70);

    QTimer *t3 = new QTimer(this);
    t3->setInterval(50);

    connect(t, SIGNAL(timeout()), this, SLOT(onEventTestTimer()));
    connect(t2, SIGNAL(timeout()), this, SLOT(onEventTestTimer()));
    connect(t3, SIGNAL(timeout()), this, SLOT(onEventTestTimer()));

//    t->start();
//    t2->start();
    //t3->start();

    connect(this, SIGNAL(transmit(DataObject*)), Air::instance(), SLOT(spreadData(DataObject*)));

    //QString menu_text;
 //   setMenuTitle(_currentMenu->text() + _currentMenu->parent() ? ((MenuItem*)_currentMenu->parent())->text() : ""  );


updateTitles();
}

void TheGUI::updateTranslation()
{
    qDebug() << "TheGUI::updateTranslation()";
    updateTitles();
//    emit menuTitleChanged();
//    emit menuSubTitleChanged();
}




void TheGUI::createMenu()
{
//    _menu << new MenuItem(
//                 "Выход",
//                 -1,
//                 nullptr,
//                 [](MenuItem *mitem, const QVariant &val) {
//                     Q_UNUSED(mitem)
//                     exit(77);
////                     qDebug() <<  "change gui theme" << mitem->text() << val;
//                 }
//             );

//!"Настройка и запуск ЛПШН"
    Menu* prepareMenu = new Menu(TRT(tr("Настройка запуска")), 0, _rootMenu);

    prepareMenu->items() << new MenuItem(TRT(tr("Типоразмер привода")), 11, new MenuItemValue(QVariant::fromValue(CustomType("rod", 1)), "", 0),
                                              [](MenuItem *mitem, const QVariant &val) {
                                                  //!todo onValueChange action
                                                   Q_UNUSED(mitem)

                                                   qDebug() << "Типоразмер привода" << val;
                                              }, prepareMenu);

    prepareMenu->items() << new MenuItem(TRT(tr("Мотор")), 12, new MenuItemValue(QVariant::fromValue(CustomType("drivepower", 1)), TRU(tr("кВт")), 0),
                                              [](MenuItem *mitem, const QVariant &val) {
                                                 Q_UNUSED(mitem)

                                                 CustomType ct(qvariant_cast<CustomType>(val));

                                                 qDebug() << "Мотор" << val << ct.toStringValue();
                                              }, prepareMenu);

    prepareMenu->items() << new MenuItem(TRT(tr("Редуктор")), 13, new MenuItemValue(QVariant::fromValue(CustomType("reducer", 1)), "", 0),
                                              [](MenuItem *mitem, const QVariant &val) {
                                                 Q_UNUSED(mitem)
                                                 //!todo onValueChange action
                                                 qDebug() << "Редуктор" << val;

                                              }, prepareMenu);

    prepareMenu->items() << new MenuItem(TRT(tr("Ограничение хода вверх")), 14, new MenuItemValue(new MenuItemValueLimiter(0.0, 0.0, 2000.0), TRU(tr("мм")), 0),
                                              [](MenuItem *mitem, const QVariant &val) {
                                                 Q_UNUSED(mitem)

                                                //!todo onValueChange action
                                                 qDebug() << "Ограничение хода вверх" << val;
                                              }, prepareMenu);

    prepareMenu->items() << new MenuItem(TRT(tr("Масимальный ток (ход вверх)")), 15, new MenuItemValue(new MenuItemValueLimiter(90.0, 0.0, 150.0), "%", 0),
                                              [](MenuItem *mitem, const QVariant &val) {
                                                  //!todo onValueChange action
                                                 Q_UNUSED(mitem)
                                                 qDebug() << "Ограничение хода вверх" << val;
                                              }, prepareMenu);

    prepareMenu->items() << new MenuItem(TRT(tr("Минимальный ток (ход вверх)")), 16, new MenuItemValue(new MenuItemValueLimiter(1.0, 0.0, 99.0), "%", 0),
                                              [](MenuItem *mitem, const QVariant &val) {
                                                  //!todo onValueChange action
                                                 Q_UNUSED(mitem)
                                                 qDebug() << "Минимальный ток (ход вверх)" << val;
                                              }, prepareMenu);

    prepareMenu->items() << new MenuItem(TRT(tr("Ручное тестирование")), 17, new MenuItemValue(QVariant::fromValue(CustomType("control", 0)), "", 0),
                                              [](MenuItem *mitem, const QVariant &val) {
                                                 Q_UNUSED(mitem)
                                                  //!todo onValueChange action
                                                 qDebug() << "Ручное тестирование" << val;
                                              }, prepareMenu);

//    prepareMenu->menu() << new MenuItem(tr("Скорость привода"), 18, new MenuItemValue(new MenuItemValueLimiter(2.0, 0.5, 14.0), "х/мин", 2),
//                                              [](MenuItem *mitem, const QVariant &val) {
//                                                 Q_UNUSED(mitem)
//                                                  //!todo onValueChange action
//                                                 qDebug() << "Число ходов в минуту" << val;
//                                              }, nullptr, prepareMenu);

//    prepareAndRunMenu->menu() << new MenuItem(tr("Число ходов в минуту"), 18, new MenuItemValue(1.1, "х/мин", 2),
//                                              [](MenuItem *mitem, const QVariant &val) {

//                                                  //!todo onValueChange action
//                                                 qDebug() << val;
//                                              }, nullptr, prepareAndRunMenu);

    _rootMenu->items() << prepareMenu;

    Menu* runMenu = new Menu(TRT(tr("Запуск")), 1, _rootMenu);

    _upDownSpeedMenu = nullptr;
    _maxUpMenu = nullptr;
    _maxDownMenu = nullptr;
    _upSpeedMenu = nullptr;
    _downSpeedMenu = nullptr;

    runMenu->items() << new MenuItem(TRT(tr("Управление")), 11,
        new MenuItemValue(QVariant::fromValue(CustomType("run", 1)), "", 0),
        [](MenuItem *mitem, const QVariant &val) {
            //!todo onValueChange action
            //!

            Q_UNUSED(mitem)

            qDebug() << val;
    }, runMenu);


    runMenu->items() << new MenuItem(TRT(tr("Режим работы")), 12,
                                              new MenuItemValue(QVariant::fromValue(CustomType("speedmode", 2)), "", 0),
                                              [](MenuItem *mitem, const QVariant &val) {
                                                  //!todo onValueChange action
                                                   Q_UNUSED(mitem)

                                                   qDebug() << val;
                                                   TheGUI::instance()->setSpeedMode(val);
                                              }, runMenu);
    _upDownSpeedMenu = new MenuItem(
                TRT(tr("Cкорость вверх/вниз")),
                13,
                new MenuItemValue(new MenuItemValueLimiter(2.0, 0.5, 10.0), TRU(tr("х/мин")), 1),
                    [](MenuItem *mitem, const QVariant &val) {

                        //!todo onValueChange action
                        Q_UNUSED(mitem)
                        bool ok = false;
                        double d = val.toDouble(&ok);
                        if (ok) {
                            DEV->setP_N1(d);
                            qDebug() << "set" << val;
                        }
                    },
                    runMenu);

    runMenu->items() << _upDownSpeedMenu;

    _maxUpMenu = new MenuItem(TRT(tr("Максимальная скорость в оптимизации")), 14,
                              new MenuItemValue(new MenuItemValueLimiter(3.0, 0.5, 10.0), TRU(tr("х/мин")), 1),
                              [](MenuItem *mitem, const QVariant &val) {
                                  //!todo onValueChange action
                                    Q_UNUSED(mitem)
                                    bool ok = false;
                                    double d = val.toDouble(&ok);
                                    if (ok) {
                                        DEV->setP_Nomax(d);
                                        qDebug() << "set" << val;
                                    }
                              }, runMenu);
    runMenu->items() << _maxUpMenu;

    _maxDownMenu =   new MenuItem(TRT(tr("Минимальная скорость в оптимизации")), 15, new MenuItemValue(new MenuItemValueLimiter(1.0, 0.5, 10.0), TRU(tr("х/мин")), 1),
                                  [](MenuItem *mitem, const QVariant &val) {
                                      //!todo onValueChange action
                                        Q_UNUSED(mitem)
                                        bool ok = false;
                                        double d = val.toDouble(&ok);
                                        if (ok) {
                                            DEV->setP_Nomin(d);
                                            qDebug() << "set" << val;
                                        }
                                  }, runMenu);

    runMenu->items() << _maxDownMenu;

    _upSpeedMenu =  new MenuItem(TRT(tr("Скорость вверх")), 16, new MenuItemValue(new MenuItemValueLimiter(2.0, 0.5, 10.0), TRU(tr("х/мин")), 1),
                                 [](MenuItem *mitem, const QVariant &val) {
                                     //!todo onValueChange action
                                        Q_UNUSED(mitem)
                                        bool ok = false;
                                        double d = val.toDouble(&ok);
                                        if (ok) {
                                            DEV->setP_N2u(d);
                                            qDebug() << "set" << val;
                                        }
                                 }, runMenu);
    runMenu->items() << _upSpeedMenu;


//    _downSpeedMenu = new MenuItem(tr("Скорость вниз"), 17, new MenuItemValue(QVariant::fromValue(DEV->P_N2d()), "", 1),
    _downSpeedMenu = new MenuItem(TRT(tr("Скорость вниз")), 17, new MenuItemValue(new MenuItemValueLimiter(2.0, 0.5, 10.0), TRU(tr("х/мин")), 1),
                                 [](MenuItem *mitem, const QVariant &val) {
                                     //!todo onValueChange action
                                        Q_UNUSED(mitem)
                                        bool ok = false;
                                        double d = val.toDouble(&ok);
                                        if (ok) {
                                            DEV->setP_N2d(d);
                                            qDebug() << "set" << val;
                                        }
                                 }, runMenu);
    runMenu->items() << _downSpeedMenu;


    runMenu->items() << new MenuItem(TRT(tr("Автоперезапуск")), 18, new MenuItemValue(QVariant::fromValue(CustomType("control", 1)), "", 0),
                                              [](MenuItem *mitem, const QVariant &val) {
                                                  //!todo onValueChange action
                                                   Q_UNUSED(mitem)
                                                   qDebug() << val;
                                              }, runMenu);

    Menu* autoRunConfMenu = new Menu(TRT(tr("Настройка автоперезапуска")), 19, runMenu);


    autoRunConfMenu->items() << new MenuItem(TRT(tr("Перегрев платы управления")), 191, new MenuItemValue(QVariant::fromValue(CustomType("control", 1)), "", 0),
                                              [](MenuItem *mitem, const QVariant &val) {
                                                  //!todo onValueChange action
                                                   Q_UNUSED(mitem)
                                                   qDebug() << val;
                                              }, autoRunConfMenu);
    setSpeedMode(2);
    runMenu->items() << autoRunConfMenu;

    _rootMenu->items() << runMenu;

//!"Параметры скважины"
    Menu *wellParamsMenu = new Menu(
                     TRT(tr("Параметры скважины")),
                     2,
                     _rootMenu
                 );

    //!********* Create Math variables submenu

    int inx = 0;
    foreach (MathVariable* mv, Mathematics::instance()->variables) {
        if (!mv->flags_str().contains("I")) {


            MenuItemValue* mival = new MenuItemValue(mv);
            MenuItem* mi = new MenuItem(
                        mv->descr(),
                        ++inx,
                        mival,
                        [](MenuItem *mitem, const QVariant &val) {
                            Mathematics::instance()->calculateExpressions();
                            Mathematics::instance()->execRTmodel(false);
                        },
                        nullptr,
                        wellParamsMenu
                    );
            wellParamsMenu->items() << mi;
        }
    }

    //!*********END Create Math variables submenu
    //!
    _rootMenu->items() << wellParamsMenu;


//! Expressions Menu
//    Menu* expressionsMenu =  new Menu(
//                TRT(tr("Расчёты")),
//                4,
//                _rootMenu
//            );
//    constructExpressionsMenu(expressionsMenu);
//    _rootMenu->items() << expressionsMenu;

    //!Связь
    Menu* netlinkMenu = new Menu(
                TRT(tr("Связь")),
                3,
                _rootMenu
            );

    netlinkMenu->items() << new MenuItem(
                                     TRT(tr("Пакетов данных в секунду")),
                                     31
                                     ,new MenuItemValue(NAN, "", 0)
                                     ,nullptr,netlinkMenu
                                 );

    netlinkMenu->items() << new MenuItem(
                                     TRT(tr("Всего пакетов в секунду")),
                                     31
                                     ,new MenuItemValue(NAN, "", 0)
                                     ,nullptr,netlinkMenu
                                 );


    _rootMenu->items() << netlinkMenu;

//    _rootMenu->menu() << new MenuItem(
//                 tr("Тестирование и проверка"),
//                 5
//             );
    Menu *graphsMenu = new Menu(
                TRT(tr("Графики и динамограммы")),
                6,
                _rootMenu
            );

    Menu *graphSettingMenu = new Menu(TRT(tr("Настройки графиков")), 61, graphsMenu);
    graphSettingMenu->items() << new MenuItem(TRT(tr("Максимальная скорость на графике")), 612,
                                             new MenuItemValue(new MenuItemValueLimiter(plotMaxV(), -1500.0, 1500.0) , TRU(tr("об/мин")), 0, true),
                                             [](MenuItem *mitem, const QVariant &val) {
                                                 Q_UNUSED(mitem)
                                                 TheGUI::instance()->setPlotMaxV(val.toDouble());
                                             },
                                             graphSettingMenu);

    graphSettingMenu->items() << new MenuItem(TRT(tr("Минимальная скорость на графике")), 613,
                                             new MenuItemValue(new MenuItemValueLimiter(plotMinV(), -1500.0, 1500.0), TRU(tr("об/мин")), 0, true),
                                             [](MenuItem *mitem, const QVariant &val) {
                                                 Q_UNUSED(mitem)
                                                 GUI->setPlotMinV(val.toDouble());
                                             },
                                             graphSettingMenu);

    graphSettingMenu->items() << new MenuItem(TRT(tr("Максимальный ток")), 614,
                                             new MenuItemValue(new MenuItemValueLimiter(plotMaxI(), -150.0, 150.0), "%", 1, true),
                                             [](MenuItem *mitem, const QVariant &val) {
                                                 Q_UNUSED(mitem)
                                                 GUI->setPlotMaxI(val.toDouble());
                                             },
                                             graphSettingMenu);

    graphSettingMenu->items() << new MenuItem(TRT(tr("Минимальный ток")), 615,
                                             new MenuItemValue(new MenuItemValueLimiter(plotMinI(), -150.0, 150.0), "%", 1, true),
                                             [](MenuItem *mitem, const QVariant &val) {
                                                 Q_UNUSED(mitem)
                                                 GUI->setPlotMinI(val.toDouble());
                                             },
                                             graphSettingMenu);

    graphSettingMenu->items() << new MenuItem(TRT(tr("Максимальное перемещение")), 616,
                                             new MenuItemValue(new MenuItemValueLimiter(plotMaxPC(), 0.0, 2000.0), TRU(tr("мм")), 0, true),
                                             [](MenuItem *mitem, const QVariant &val) {
                                                 Q_UNUSED(mitem)
                                                 GUI->setPlotMaxPC(val.toDouble());
                                             },
                                             graphSettingMenu);

    graphSettingMenu->items() << new MenuItem(TRT(tr("Минимальное перемещение")), 617,
                                             new MenuItemValue(new MenuItemValueLimiter(plotMinPC(), 0.0, 200.0), TRU(tr("мм")), 0, true),
                                             [](MenuItem *mitem, const QVariant &val) {
                                                 Q_UNUSED(mitem)
                                                 GUI->setPlotMinPC(val.toDouble());
                                             },
                                             graphSettingMenu);
    graphsMenu->items() << graphSettingMenu;


    Menu *dynamoSettingMenu = new Menu(TRT(tr("Настройки динамограмм")), 62, graphsMenu);

    dynamoSettingMenu->items() << new MenuItem(TRT(tr("Максимальное усилие")), 621,
                                              new MenuItemValue(new MenuItemValueLimiter(dynamoMaxFkg(), 0.0, 25000.0), TRU(tr("кг")), 0, true),
                                              [](MenuItem *mitem, const QVariant &val) {
                                                  Q_UNUSED(mitem)
                                                  GUI->setDynamoMaxFkg(val.toDouble());
                                              },
                                              dynamoSettingMenu);


    dynamoSettingMenu->items() << new MenuItem(TRT(tr("Минимальное усилие")), 622,
                                              new MenuItemValue(new MenuItemValueLimiter(dynamoMinFkg(), -200.0, 10000.0), TRU(tr("кг")), 0, true),
                                              [](MenuItem *mitem, const QVariant &val) {
                                                  Q_UNUSED(mitem)
                                                  GUI->setDynamoMinFkg(val.toDouble());
                                              },
                                              dynamoSettingMenu);

    dynamoSettingMenu->items() << new MenuItem(TRT(tr("Максимальное перемещение")), 623,
                                              new MenuItemValue(new MenuItemValueLimiter(dynamoMaxPCmm(), 0.0, 2000.0), TRU(tr("мм")), 0),
                                              [](MenuItem *mitem, const QVariant &val) {
                                                  Q_UNUSED(mitem)
                                                  GUI->setDynamoMaxPCmm(val.toDouble());
                                              },
                                              dynamoSettingMenu);
    dynamoSettingMenu->items() << new MenuItem(TRT(tr("Минимальное перемещение")), 624,
                                              new MenuItemValue(new MenuItemValueLimiter(dynamoMinPCmm(), 0.0, 200.0), TRU(tr("мм")), 0),
                                              [](MenuItem *mitem, const QVariant &val) {
                                                  Q_UNUSED(mitem)
                                                  GUI->setDynamoMinPCmm(val.toDouble());
                                              },
                                              dynamoSettingMenu);
    graphsMenu->items() << dynamoSettingMenu;


    _rootMenu->items() << graphsMenu;

    _rootMenu->items() << new MenuItem(
                 TRT(tr("Аварии и события")),
                 7
             );

    _rootMenu->items() << new MenuItem(
                 TRT(tr("Архив и параметры архивирования")),
                 8
             );

    Menu *serviceMenu =  new Menu(
                TRT(tr("Сервис")),
                9,
                _rootMenu
            );


    serviceMenu->items() << new MenuItem(
                                     TRT(tr("Поиск обновлений и установка ПО")),
                                     91
                                     ,nullptr,
                                     [](MenuItem *mitem, const QVariant &val) {
                                        Q_UNUSED(mitem);

//                                        QGuiApplication::exit(77);
                                        // fast close
#ifdef DESKTOP
                                        quick_exit(77);
#endif

#ifdef SMH4
                                        quick_exit(77);
#elif MACX
                                        exit(77);
#endif

                                     }
                                     ,serviceMenu
                                 );

//    Menu *vfcconMenu =  new Menu(
//                TRT(tr("Консоль ПЧ")),
//                92,
//                serviceMenu
//            );

//    vfcconMenu->menu() << new MenuItem(
//                                     TRT(tr("Параметр 1")),
//                                     921
//                                     ,nullptr,nullptr,vfcconMenu
//                                 );

//    vfcconMenu->menu() << new MenuItem(
//                                     tr("Параметр 2"),
//                                     922
//                                     ,nullptr,nullptr,nullptr,vfcconMenu
//                                 );

//    vfcconMenu->menu() << new MenuItem(
//                                     tr("Отправить команду"),
//                                     923
//                                     ,nullptr,nullptr,nullptr,vfcconMenu
//                                 );

//    serviceMenu->menu() << vfcconMenu;

    serviceMenu->items() << new MenuItem(
                 TRT(tr("Интерфейс")),
                 93,
                 new MenuItemValue(QVariant::fromValue(CustomType("theme", 1)), "", 0),
                 [](MenuItem *mitem, const QVariant &val) {
                     Q_UNUSED(mitem)
                     CustomType ct(qvariant_cast<CustomType>(val));
                     TheGUI::instance()->setGuiDark(ct.value() == 0);

                 }
             );

    serviceMenu->items() << new MenuItem(
        TRT(tr("Язык")),
        94,
        new MenuItemValue(QVariant::fromValue(CustomType("lang", 1)), TRU(tr("Язык")), 0),
        [=](MenuItem *mitem, const QVariant &val) {
            Q_UNUSED(mitem)
            CustomType ct(qvariant_cast<CustomType>(val));

            QMap<int, QString> m({{0, "ru"}, {1, "en"}, {2, "jp"}, {3, "id"} });
            TranslateSwitcher::instance()->changeTs(m[ct.value()]);

            ITrObj::updateTrs();

            setMenu(nullptr);
            //TheGUI::instance()->setMenu(nullptr);
        }
    );



    _rootMenu->items() << serviceMenu;
//    _rootMenu->menu() << new MenuItem(
//                 TRT(tr("Скорость")),
//                 1011,
//                 new MenuItemValue(1.5, TRU(tr("х/мин")), 1),
//                 [](MenuItem *mitem, const QVariant &val) {
//                     qDebug() <<  "labmda" << mitem->text() << val;
//                 }

//             );


//    _rootMenu->menu() << new MenuItem(TRT(tr("Первая строка с длинным текстом который, по идее, должен перекрыть короткое значение")), 12345, new MenuItemValue(100.15, TRU(tr("штук")), 3));

//    _rootMenu->menu() << new MenuItem(TRT(tr("Скорость")), -1, new MenuItemValue(21285406.12543, TRU(tr("цифер")), 2));
//    _rootMenu->menu() << new MenuItem(TRT(tr("Интерфейс")), 303, new MenuItemValue(6.12343, TRU(tr("чисел")), 3));


//    _rootMenu->menu() << new MenuItem(TRT(tr("Профиль")), 1012, new MenuItemValue(QVariant::fromValue(CustomType("rod", 1)), "", 0));

//    _rootMenu->menu() << new MenuItem("Test1", 5012, new MenuItemValue(QVariant::fromValue(CustomType("bool", 0)), "", 0));
//    _rootMenu->menu() << new MenuItem("Test2", 5013, new MenuItemValue(QVariant::fromValue(CustomType("noyes", 1)), "", 0));
//    _rootMenu->menu() << new MenuItem("Test3", 5014, new MenuItemValue(QVariant::fromValue(CustomType("enum", 2)), "", 0));
//    _rootMenu->menu() << new MenuItem("Test3", 5015, new MenuItemValue(QVariant::fromValue(CustomType("control", 0)), "", 0));
//    _rootMenu->menu() << new MenuItem("Test4 строка с длинным текстом который, по идее, должен перекрыть короткое значение", 5015, new MenuItemValue(QVariant::fromValue(CustomType("drivepower", 1)), "", 0));

    //QVariant::fromValue(CustomType("rod", 0))

//    int k=3000;
//    for (double d = -1000000.0; d<=1000000; d+=9999) {
//        _rootMenu->menu() << new MenuItem(TRT(tr("Первая строка с длинным текстом который, по идее, должен перекрыть короткое значение")), ++k, new MenuItemValue(d, TRU(tr("штук")), 2));
//    }

//    _rootMenu->menu() << new MenuItem(QString::fromUtf8("пывап"), 7980);
//    _rootMenu->menu() << new MenuItem("Third", 0156);
//    for (int i=0;i<1000;++i) {
//        _rootMenu->menu() << new MenuItem(QString("Menu entry %1").arg(i), 1000 + i);

//    }

    }

void TheGUI::hideNotifierAndCleanLastEvents()
{
    qDebug() << "TheGUI::hideNotifierAndCleanLastEvents";
    setIsNotifierVisible(false);

    eventsModel()->cleanLastEvents();
}

void TheGUI::showNotifier()
{
    qDebug() << "TheGUI::showNotifier";
    setIsNotifierVisible(true);
}



void TheGUI::initRing1()
{
    mLog() << "GUIHelper::initRing1";

    eventsModel()->loadFile();

    //DynamicMenuTypes::instance()->loadTypes();
}

double procData(ulong value)
{
    qDebug() << "GUIHelper::procData" << value;


    ulong sleepTime = value / 100;
    qDebug()  <<  QThread::currentThread() << " slee§p  for" << sleepTime;
    QThread::currentThread()->sleep(sleepTime);
    qDebug()  <<  QThread::currentThread() << " end sleep" << sleepTime;

    return value / 100.0;
}

void TheGUI::testFun()
{
    qDebug() << "GUIHelper::testFun";

    QList<int> inData;
    inData << 100;
    inData << 200;
    inData << 300;
    inData << 100;
    inData << 200;
    inData << 300;
    inData << 100;
    inData << 200;
    inData << 300;
    inData << 100;
    inData << 200;
    inData << 300;
    inData << 100;
    inData << 100;
    inData << 100;

//    QFuture<double> results = QtConcurrent::mapped(inData, procData);
//    _watcher->setFuture(results);
    //QThread::currentThread()->sleep(10);
}

void TheGUI::menuUp()
{
    Menu *oldMenu = _currentMenu;
    Menu *parentMenu = dynamic_cast<Menu*>(_currentMenu->parent());
    if (parentMenu) {
        setMenu(parentMenu);
        int inx = _currentMenu->items().indexOf(oldMenu);
        emit changeCurrentIndex(inx);
    }
}

void TheGUI::switchPlotVisible()
{
    qDebug() << "TheGUI::switchPlotVisible()";
    setDynamoVisible(false);
    setPlotVisible(true);
}

void TheGUI::switchDynamoVisible()
{
    qDebug() << "TheGUI::switchDynamoVisible()";
    setPlotVisible(false);
    setDynamoVisible(true);
}

void TheGUI::selectFirstEvent()
{
    emit selectEvent(0);
}

void TheGUI::log(const QString &str)
{
    mLog() << QString("qml: %1").arg(str);
}

bool TheGUI::isMenuEditNow()
{
    return MenuItem::s_editNowMenuItem;
}

void TheGUI::setMenu(Menu *menu)
{
    if (menu == nullptr) {
        menu = _rootMenu;
    }

    _currentMenu = menu;
    setMenuTitle(_currentMenu->text());

    if (_currentMenu->parent()) {
        MenuItem* mi = dynamic_cast<MenuItem*>(_currentMenu->parent());
        setMenuSubTitle(mi->text());
    } else {
        setMenuSubTitle("");
    }

    emit menuChanged();
}

const QList<QObject *>& TheGUI::menu() const
{
    return _currentMenu->items();
}

QString TheGUI::menuTitle()
{
    return _menuTitle;
}

void TheGUI::setMenuTitle(const QString &userName)
{
    if (userName == _menuTitle)
           return;

   _menuTitle = userName;
   emit menuTitleChanged();

}

TranslateSwitcher *TheGUI::tswitcher()
{
    return &_tswitch;
}

void TheGUI::notifyMenuEditNowChanged()
{
    emit isMenuEditNowChanged();
}

double TheGUI::dynamoMinPCmm() const
{
    return _dynamoMinPCmm;
}

void TheGUI::setDynamoMinPCmm(double dynamoMinPCmm)
{
    _dynamoMinPCmm = dynamoMinPCmm;
}

double TheGUI::dynamoMaxPCmm() const
{
    return _dynamoMaxPCmm;
}

void TheGUI::setDynamoMaxPCmm(double dynamoMaxPCmm)
{
    if (cmpFV(_dynamoMaxPCmm, dynamoMaxPCmm)) return;
    _dynamoMaxPCmm = dynamoMaxPCmm;
    emit dynamoMaxPCChanged();

}

double TheGUI::dynamoMinFkg() const
{
    return _dynamoMinFkg;
}

void TheGUI::setDynamoMinFkg(double dynamoMinFkg)
{
    if (cmpFV(_dynamoMinFkg, dynamoMinFkg)) return;
    _dynamoMinFkg = dynamoMinFkg;
    emit dynamoMinFChanged();
}

double TheGUI::dynamoMaxFkg() const
{
    return _dynamoMaxFkg;
}

void TheGUI::setDynamoMaxFkg(double dynamoMaxFkg)
{
    if (cmpFV(_dynamoMaxFkg, dynamoMaxFkg)) return;
    _dynamoMaxFkg = dynamoMaxFkg;
    emit dynamoMaxFChanged();
}

bool TheGUI::dynamoVisible() const
{
    return _dynamoVisible;
}

void TheGUI::setDynamoVisible(bool dynamoVisible)
{
    if (_dynamoVisible == dynamoVisible) return;
    _dynamoVisible = dynamoVisible;
    emit dynamoVisibleChanged();
}

bool TheGUI::plotVisible() const
{
    return _plotVisible;
}

void TheGUI::setPlotVisible(bool plotVisible)
{
    if (_plotVisible == plotVisible) return;
    _plotVisible = plotVisible;
    emit plotVisibleChanged();
}

double TheGUI::plotMinPC() const
{
    return _plotMinPCmm;
}

void TheGUI::setPlotMinPC(double plotMinPC)
{
    if (cmpFV(_plotMinPCmm, plotMinPC)) return;
    _plotMinPCmm = plotMinPC;
    emit plotMinPCChanged();
}

double TheGUI::plotMaxPC() const
{
    return _plotMaxPCmm;
}

void TheGUI::setPlotMaxPC(double plotMaxPC)
{
    if (cmpFV(_plotMaxPCmm, plotMaxPC)) return;
    _plotMaxPCmm = plotMaxPC;
    emit plotMaxPCChanged();
}

double TheGUI::plotMinI() const
{
    return _plotMinIp;
}

void TheGUI::setPlotMinI(double plotMinI)
{
    if (cmpFV(_plotMinIp, plotMinI)) return;
    _plotMinIp = plotMinI;
    emit plotMinIChanged();
}

double TheGUI::plotMaxI() const
{
    return _plotMaxIp;
}

void TheGUI::setPlotMaxI(double plotMaxI)
{
    if (cmpFV(_plotMaxIp, plotMaxI)) return;
    _plotMaxIp = plotMaxI;
    emit plotMaxIChanged();
}

double TheGUI::plotMinV() const
{
    return _plotMinVrpm;
}

void TheGUI::setPlotMinV(double plotMinV)
{
    if (cmpFV(_plotMinVrpm, plotMinV)) return;
    _plotMinVrpm = plotMinV;
    emit plotMinVChanged();
}

double TheGUI::plotMaxV() const
{
    return _plotMaxVrpm;
}

void TheGUI::setPlotMaxV(double plotMaxV)
{
    if (cmpFV(_plotMaxVrpm, plotMaxV)) return;
    _plotMaxVrpm = plotMaxV;
    emit plotMaxVChanged();
}

void TheGUI::setIsNotifierVisible(bool isNotifierVisible)
{
    if (_isNotifierVisible == isNotifierVisible) return;

    _isNotifierVisible = isNotifierVisible;
    emit isNotifierVisibleChanged();
}

QString TheGUI::menuSubTitle() const
{
    return _menuSubTitle;
}

void TheGUI::setMenuSubTitle(const QString &menuSubTitle)
{
    if (_menuSubTitle == menuSubTitle) return;
    _menuSubTitle = menuSubTitle;
    emit menuSubTitleChanged();
}

void TheGUI::constructExpressionsMenu(Menu *menu)
{
    int inx = 0;
    foreach (MathExpression* mv, Mathematics::instance()->expressions) {
        MenuItemValue* mival = new MenuItemValue(mv);
        MenuItem* mi = new MenuItem(
                    mv->descr(),
                    ++inx,
                    mival,
                    nullptr,
                    nullptr,
                    menu
                );
        mv->_mitem = mi;
        menu->items() << mi;
    }

}

void TheGUI::setSpeedMode(const QVariant &mode)
{
    qDebug()  << "TheGUI::setSpeedMode" << mode;
    if(mode.type() == QVariant::Int) {
        DEV->setSpeedMode(mode.toInt());
        return;
    }

    CustomType ct(qvariant_cast<CustomType>(mode));
    switch(ct.value()) {
    case 0: {
        _upDownSpeedMenu->setEnabled(false);
        _maxUpMenu->setEnabled(true);
        _maxDownMenu->setEnabled(true);
        _upSpeedMenu->setEnabled(false);
        _downSpeedMenu->setEnabled(false);
    }; break;
    case 1: {
        _upDownSpeedMenu->setEnabled(true);
        _maxUpMenu->setEnabled(false);
        _maxDownMenu->setEnabled(false);
        _upSpeedMenu->setEnabled(false);
        _downSpeedMenu->setEnabled(false);
    }; break;
    case 2: {
        _upDownSpeedMenu->setEnabled(false);
        _maxUpMenu->setEnabled(false);
        _maxDownMenu->setEnabled(false);
        _upSpeedMenu->setEnabled(true);
        _downSpeedMenu->setEnabled(true);
    }; break;

    }
    DEV->setSpeedMode(ct.value());
}

bool TheGUI::isNotifierVisible()
{
    return _isNotifierVisible;
}

void TheGUI::onEventTestTimer()
{
    //    qDebug() << "GUIHelper::onEventTestTimer";
    //    emit eventsChanged();

    SMSG_I(QString("%1 <font color=\"#ff7f00\">abcdefghijklopqrastuvqxyz съешь ещё этих мягких французских булок да выпей же чаю</font>" ).arg(++_i_t));
}

void TheGUI::onLangChanged()
{
    //qDebug() << "GUIHelper::onLangChanged";
    emit menuChanged();
    emit eventsChanged();
}

void TheGUI::updateTitles()
{
    _currentMenu->updateTranslation();
    setMenuTitle(_currentMenu->text());
    if (_currentMenu->parent()) {
        MenuItem* mi = dynamic_cast<MenuItem*>(_currentMenu->parent());
        mi->updateTranslation();
        setMenuSubTitle(mi->text());
    } else {
        setMenuSubTitle("");
    }
}


QThread *TheGUI::qmlThread() const
{
    return _qmlThread;
}

void TheGUI::setQmlThread(QThread *qmlThread)
{
    _qmlThread = qmlThread;
}


bool TheGUI::isGuiDark() const
{
    return _isGuiDark;
}

void TheGUI::setGuiDark(bool value)
{
    if (_isGuiDark == value) return;

    _isGuiDark = value;
    emit guiDarkChanged();
}

QString TheGUI::statusBarText() const
{
    return _statusBarText;
}

void TheGUI::setStatusBarText(const QString &statusBarText)
{
    if (_statusBarText == statusBarText) return;

    _statusBarText = statusBarText;
    emit statusBarTextChanged();
}

QAbstractItemModel *TheGUI::events() const
{
    return _events;
}

void TheGUI::sayUpdateScrollBar()
{
    emit updateScrollBarEvent();
}

EventsModel *TheGUI::eventsModel() const
{
    return _events;
}

bool TheGUI::clksynced() const
{
    return _clksynced;
}


