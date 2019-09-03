
#include "view.h"
#include "sgviewcamera.h"
#include "scene.h"
#include "scenefantacy.h"
#include "scenevideo.h"

#include <QApplication>
#include <QVBoxLayout>


int main(int argc, char **argv)
{
    QSurfaceFormat format;
    format.setSamples(4);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication app(argc, argv);

    SceneFantacy scene;
    //Scene scene;
    //SceneVideo scene;
    
    View view;
    view.attach(&scene);
    view.viewCamera()->setFillMode(true);

    View slaveView;
    slaveView.attach(&scene);

    QWidget mainwindow;
    QVBoxLayout layout;
    layout.addWidget(&view);
    layout.addWidget(&slaveView);
    mainwindow.setLayout(&layout);
    mainwindow.setMinimumSize(640, 480);
    mainwindow.show();

    return app.exec();
}
