
#include "src/sgview.h"
#include "scene.h"

#include <QGuiApplication>


//! [2]
int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);

    Scene scene;
    
    SGView window;
    window.setScene(&scene);
    window.setFormat(format);
    window.resize(640, 480);
    window.initialize();
    window.show();

    return app.exec();
}
