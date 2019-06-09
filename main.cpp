
#include "sgscene.h"
#include "sgview.h"
#include <QGuiApplication>


//! [1]

//! [2]
int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);

    SGScene scene;
    scene.build();

    SGView window;
    window.setScene(&scene);
    window.setFormat(format);
    window.resize(640, 480);
    window.show();

    return app.exec();
}
