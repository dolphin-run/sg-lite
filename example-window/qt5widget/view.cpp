#include "view.h"
#include <QOpenGLContext>

static QOpenGLContext *g_context = nullptr;


View::View(QWidget *parent)
    : QOpenGLWidget(parent)
    , m_context(0)
{
    m_pUserInteraction = new ViewInteraction(this);
    this->installEventFilter(m_pUserInteraction);
}


View::~View()
{
}

void View::initializeGL()
{
    if (!g_context)
    {
        g_context = QOpenGLContext::currentContext();
    }
    else
    {
        QOpenGLContext::currentContext()->setShareContext(g_context);
        QOpenGLContext::currentContext()->create();
    }
}

void View::resizeGL(int w, int h)
{
    m_camera.setViewSize(w, h);
}

void View::paintGL()
{
    if (!isVisible())
        return;
    SGView::renderFrame();

    update();
}

float View::widthPx() const
{
    return width()*QWidget::devicePixelRatio();
}

float View::heightPx() const
{
    return height()*QWidget::devicePixelRatio();
}

void View::CameraMove(float x, float y)
{
    m_camera.move(x, y);
}

void View::CameraScale(bool zoom)
{
    if (zoom)
    {
        m_camera.scale(1.01f);
    }
    else
    {
        m_camera.scale(1.f / 1.01f);
    }
}