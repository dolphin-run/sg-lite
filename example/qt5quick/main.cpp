
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "fbo.h"


int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);


    qmlRegisterType<SceneQuickItem>("SGUtil", 1, 0, "SGView");

    QQmlApplicationEngine engine;
    engine.load("assets/qml/main.qml");
    if (engine.rootObjects().isEmpty())
    {
        return 1;
    }

    return app.exec();
}
