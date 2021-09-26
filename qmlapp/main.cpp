#include <QObject>
#include <QQmlEngine>
#include <QJSEngine>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFontDatabase>
#include <QThread>
#include <QAbstractSocket>


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    engine.rootContext()->setContextProperty("fixedFont", fixedFont);
    qmlRegisterSingletonType(QUrl("qrc:/Globals.qml"), "dev.pump.station.gui", 1, 0, "Globals");

    engine.load(QUrl(QStringLiteral("qrc:/app.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    int retCode = app.exec();

    return retCode;
}
