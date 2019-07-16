
#include "view.h"
#include "scene.h"
#include "scenefantacy.h"

#include <QGuiApplication>


int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);

    //SceneFantacy scene;
    Scene scene;
    
    View window;
    window.attach(&scene);
    window.setFormat(format);
    window.setMinimumSize(QSize(640, 480));
    window.showMaximized();

    //View window1;
    //window1.attach(&scene);
    //window1.setFormat(format);
    //window1.setMinimumSize(QSize(640, 480));
    //window1.show();

    return app.exec();
}
