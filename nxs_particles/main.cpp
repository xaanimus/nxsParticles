#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "mainview.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<MainView>("MainLib", 1, 0, "MainView");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
