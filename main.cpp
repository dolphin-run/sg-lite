
#include "src/sgview.h"
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
    
    SGView window;
    window.setScene(&scene);
    window.setFormat(format);
    window.resize(640, 480);
    window.initialize();
    window.show();

    return app.exec();
}
