#include <QDebug>
#include <QLibraryInfo>
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

    qmlRegisterSingletonType(QUrl("qrc:/Globals.qml"), "globals.gui.qmlapp", 1, 0, "Globals");
    qmlRegisterSingletonType(QUrl("qrc:/SMH4.qml"), "globals.gui.qmlapp", 1, 0, "SMH4");

    engine.load(QUrl(QStringLiteral("qrc:/app.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    qDebug() << "QLibraryInfo::PluginsPath" << QLibraryInfo::location(QLibraryInfo::PluginsPath);


    int retCode = app.exec();

    return retCode;
}
