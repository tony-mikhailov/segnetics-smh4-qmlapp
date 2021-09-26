#ifndef THEGUI_H
#define THEGUI_H

#include <QObject>
//#include <QtConcurrent>
#include <QList>
#include <QDateTime>
#include <QAbstractItemModel>
#include <QPointer>

#include "itrobj.h"
#include "event.h"
#include "translateswitcher.h"
#include "dataobject.h"

class EventsModel;
class MenuItem;
class Menu;

class TheGUI : public QObject, public ITrObj
{
    Q_OBJECT

public:

    static QVariant getGraphLims(MenuItem *miv, const QString &name);
    static void setGraphLims(MenuItem *miv, const QString &name, const QVariant &value);

    static TheGUI *instance();
    explicit TheGUI(QObject *parent = nullptr);

    void updateTranslation();

    Q_PROPERTY(QString menuTitle READ menuTitle WRITE setMenuTitle NOTIFY menuTitleChanged)
    Q_PROPERTY(QString menuSubTitle READ menuSubTitle WRITE setMenuSubTitle NOTIFY menuSubTitleChanged)

    Q_PROPERTY(QList<QObject*> menu READ menu NOTIFY menuChanged)
    Q_PROPERTY(bool isGuiDark READ isGuiDark WRITE setGuiDark NOTIFY guiDarkChanged)

    //что происходит когда эмитится eventsChanged? ;)
    Q_PROPERTY(QAbstractItemModel* events READ events NOTIFY eventsChanged)
    Q_PROPERTY(bool isMenuEditNow READ isMenuEditNow NOTIFY isMenuEditNowChanged)

    Q_PROPERTY(bool isNotifierVisible READ isNotifierVisible NOTIFY isNotifierVisibleChanged)

    Q_PROPERTY(double plotMaxV READ plotMaxV WRITE setPlotMaxV NOTIFY plotMaxVChanged)
    Q_PROPERTY(double plotMinV READ plotMinV WRITE setPlotMinV NOTIFY plotMinVChanged)
    Q_PROPERTY(double plotMaxI READ plotMaxI WRITE setPlotMaxI NOTIFY plotMaxIChanged)
    Q_PROPERTY(double plotMinI READ plotMinI WRITE setPlotMinI NOTIFY plotMinIChanged)
    Q_PROPERTY(double plotMaxPC READ plotMaxPC WRITE setPlotMaxPC NOTIFY plotMaxPCChanged)
    Q_PROPERTY(double plotMinPC READ plotMinPC WRITE setPlotMinPC NOTIFY plotMinPCChanged)

    Q_PROPERTY(double dynamoMaxF READ dynamoMaxFkg WRITE setDynamoMaxFkg NOTIFY dynamoMaxFChanged)
    Q_PROPERTY(double dynamoMinF READ dynamoMinFkg WRITE setDynamoMinFkg NOTIFY dynamoMinFChanged)
    Q_PROPERTY(double dynamoMaxPC READ dynamoMaxPCmm WRITE setDynamoMaxPCmm NOTIFY dynamoMaxPCChanged)
    Q_PROPERTY(double dynamoMinPC READ dynamoMinPCmm WRITE setDynamoMinPCmm NOTIFY dynamoMinPCChanged)

    Q_PROPERTY(bool plotVisible READ plotVisible WRITE setPlotVisible NOTIFY plotVisibleChanged)
    Q_PROPERTY(bool dynamoVisible READ dynamoVisible WRITE setDynamoVisible NOTIFY dynamoVisibleChanged)

    void createMenu();

    Q_INVOKABLE void hideNotifierAndCleanLastEvents();
    Q_INVOKABLE void showNotifier();


    Q_INVOKABLE void testFun();
    Q_INVOKABLE void initRing1();
    Q_INVOKABLE void log(const QString &str);

    Q_INVOKABLE void menuUp();

    Q_INVOKABLE void switchPlotVisible();
    Q_INVOKABLE void switchDynamoVisible();


    Q_INVOKABLE void selectFirstEvent();
    Q_INVOKABLE void sayUpdateScrollBar();
    Q_INVOKABLE QAbstractItemModel *events() const;
    EventsModel *eventsModel() const;

    bool isMenuEditNow();
    void setMenu(Menu *menu);
    const QList<QObject *> &menu() const;

    QString menuTitle();
    void setMenuTitle(const QString &menuTitle);

    TranslateSwitcher* tswitcher();

    friend class MenuItem;
    friend class Menu;

    bool clksynced() const;

    QString statusBarText() const;
    void setStatusBarText(const QString &statusBarText);

    bool isGuiDark() const;
    void setGuiDark(bool value);

    QThread *qmlThread() const;
    void setQmlThread(QThread *qmlThread);

    bool vfcConnected() const;
    bool gatewayConnected() const;

    QString menuSubTitle() const;
    void setMenuSubTitle(const QString &menuSubTitle);
    void constructExpressionsMenu(Menu* menu);

    void setSpeedMode(const QVariant &mode);

    bool isNotifierVisible();

    void setIsNotifierVisible(bool isNotifierVisible);

    double plotMaxV() const;
    void setPlotMaxV(double plotMaxV);
    double plotMinV() const;
    void setPlotMinV(double plotMinV);

    double plotMaxI() const;
    void setPlotMaxI(double plotMaxI);
    double plotMinI() const;
    void setPlotMinI(double plotMinI);

    double plotMaxPC() const;
    void setPlotMaxPC(double plotMaxPC);
    double plotMinPC() const;
    void setPlotMinPC(double plotMinPC);

    bool plotVisible() const;
    void setPlotVisible(bool plotVisible);

    bool dynamoVisible() const;
    void setDynamoVisible(bool dynamoVisible);

    double dynamoMaxFkg() const;
    void setDynamoMaxFkg(double dynamoMaxFkg);

    double dynamoMinFkg() const;
    void setDynamoMinFkg(double dynamoMinFkg);

    double dynamoMaxPCmm() const;
    void setDynamoMaxPCmm(double dynamoMaxPCmm);

    double dynamoMinPCmm() const;
    void setDynamoMinPCmm(double dynamoMinPCmm);

signals:
    void menuTitleChanged();
    void statusBarTextChanged();
    void guiDarkChanged();

    void menuChanged();
    void eventsChanged();
    void isMenuEditNowChanged();
    void changeCurrentIndex(int idx);
    void selectEvent(int idx);
    void updateScrollBarEvent();

    void transmit(DataObject* data);
    void vfcConnectedChanged();
    void gatewayConnectedChanged();
    void menuSubTitleChanged();
    void isNotifierVisibleChanged();

    void plotMaxVChanged();
    void plotMinVChanged();
    void plotMaxIChanged();
    void plotMinIChanged();
    void plotMaxPCChanged();
    void plotMinPCChanged();

    void dynamoMaxFChanged();
    void dynamoMinFChanged();
    void dynamoMaxPCChanged();
    void dynamoMinPCChanged();


    void dynamoVisibleChanged();
    void plotVisibleChanged();



public slots:
    void onEventTestTimer();
    void onLangChanged();
    void updateTitles();

protected:
    static TheGUI *g_gui;

private:
    void notifyMenuEditNowChanged();

private:
    QString _menuTitle;
    QString _statusBarText;
    bool _isGuiDark;

    Menu *_rootMenu;
    Menu *_currentMenu;

    TranslateSwitcher _tswitch;
    bool _clksynced = false;

    EventsModel *_events;
    QThread *_qmlThread = nullptr;
    int _i_t = 0;
    QString _menuSubTitle;

    MenuItem* _upDownSpeedMenu = nullptr;
    MenuItem* _maxUpMenu = nullptr;
    MenuItem* _maxDownMenu = nullptr;
    MenuItem* _upSpeedMenu = nullptr;
    MenuItem* _downSpeedMenu = nullptr;

    bool _isNotifierVisible = false;


    double _plotMaxVrpm = 1500.0;
    double _plotMinVrpm = -1500.0;
    double _plotMaxIp = 150.0;
    double _plotMinIp = -150.0;
    double _plotMaxPCmm = 2000.0;
    double _plotMinPCmm = 0;

//    double _dynamoMaxFkg = 15000.0;
    double _dynamoMaxFkg = 10000.0;
    double _dynamoMinFkg = -150.0;
    double _dynamoMaxPCmm = 2000.0;
    double _dynamoMinPCmm = 0;


    //    double _plotMaxF = 40000.0;
    //    double _plotMinF = -500.0;
    bool _plotVisible = false;
    bool _dynamoVisible = true;
};

#define GUI TheGUI::instance()

#endif // THEGUI_H
