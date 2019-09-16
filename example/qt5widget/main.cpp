
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
    SceneVideo scene1;
    
    View *view = new View;
    view->attach(&scene);
    view->viewCamera()->setFillMode(true);

    View *slaveView = new View;
    slaveView->attach(&scene1);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(view);
    layout->addWidget(slaveView);

    QWidget mainwindow;
    mainwindow.setLayout(layout);
    mainwindow.setMinimumSize(640, 480);
    mainwindow.show();

    return app.exec();
}
