#include "view.h"
#include "sgviewcamera.h"
#include <QOpenGLContext>


View::View(QWidget *parent)
    : QOpenGLWidget(parent)
{
}


View::~View()
{
}

void View::initializeGL()
{
}

void View::resizeGL(int w, int h)
{
    int radio = devicePixelRatio();
    SGView::setSize(w, h);
    SGView::setSizePx(w*radio, h*radio);
}

void View::paintGL()
{
    if (!isVisible())
        return;
    SGView::renderFrame();

    update();
}