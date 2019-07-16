#include "view.h"
#include <QOpenGLContext>
#include <QMouseEvent>
#include "src/node/sgrectanglenode.h"
#include "sgscene.h"



void View::renderLater()
{
    requestUpdate();
}

bool View::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::UpdateRequest:
        render();
        return true;
    default:
        return QWindow::event(event);
    }
}

void View::resizeEvent(QResizeEvent * event)
{
    m_camera.setViewSize(this->width(), this->height());
}

void View::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);

    if (isExposed())
        render();
}

void View::render()
{
    if (!isExposed())
        return;

    bool needsInitialize = false;

    if (!m_context) {
        m_context = new QOpenGLContext(this);
        m_context->setFormat(requestedFormat());
        m_context->create();

        needsInitialize = true;
    }

    m_context->makeCurrent(this);

    if (needsInitialize)
        SGView::initialize();
    SGView::renderFrame();

    m_context->swapBuffers(this);

    renderLater();
}

void View::mousePressEvent(QMouseEvent * ev)
{
    auto pos = ev->pos();
    Point pt;
    pt.set(pos.x(), pos.y());
    pt = mapToScene(pt);
    
    SGRectangleNode *sn = new SGRectangleNode();
    sn->setX(pt.x);
    sn->setY(pt.y);
    sn->setZ(1);
    sn->setWidth(150);
    sn->setHeight(100);

    sn->setColor(Color::random());

    m_scene->addItem(sn);
}

View::View(QWindow *parent)
    : QWindow(parent)
    , m_context(0)
{
    setSurfaceType(QWindow::OpenGLSurface);

    m_pUserInteraction = new ViewInteraction(this);
    this->installEventFilter(m_pUserInteraction);
}


View::~View()
{
}

int View::width() const
{
    return QWindow::width();
}

int View::height() const
{
    return QWindow::height();
}

float View::pixelRatio() const
{
    return QWindow::devicePixelRatio();
}

void View::CameraMove(int x, int y)
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