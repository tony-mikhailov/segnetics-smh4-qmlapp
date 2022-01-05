#include <QGuiApplication>
#include <QQmlApplicationEngine>


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterSingletonType(QUrl("qrc:/Globals.qml"), "globals.gui.qmlapp", 1, 0, "Globals");
    qmlRegisterSingletonType(QUrl("qrc:/SMH4.qml"), "globals.gui.qmlapp", 1, 0, "SMH4");

    engine.load(QUrl(QStringLiteral("qrc:/app.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
