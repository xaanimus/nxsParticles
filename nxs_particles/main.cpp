#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "mainview.h"
#include "framebufferitem.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<MainView>("MainLib", 1, 0, "MainView");
    qmlRegisterType<FrameBufferItem>("MainLib", 1, 0, "FBItem");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
