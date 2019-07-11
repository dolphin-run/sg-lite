#include "sgview.h"
#include <QScreen>
#include <QWheelEvent>
#include "sgnode.h"
#include "sgscene.h"
#include "sgrenderer.h"


SGView::SGView(QWindow *parent)
    : QWindow(parent)
    , m_context(0)
{
    setSurfaceType(QWindow::OpenGLSurface);

    m_render = new SGRenderer(this);
}

SGView::~SGView()
{
}

void SGView::initialize()
{
    m_pUserInteraction = new ViewInteraction(this);
    this->installEventFilter(m_pUserInteraction);

    m_viewPortCenter.x = 960;
    m_viewPortCenter.y = 540;
}

void SGView::setScene(SGScene *scene)
{
    m_scene = scene;
}

const Rect & SGView::viewRect() const
{
    return m_viewPortRect;
}

const glm::mat4 & SGView::transform() const
{
    return m_viewTransform;
}

void SGView::CameraMove(int x, int y)
{
    m_viewPortCenter.x += (x / m_viewPortScale);
    m_viewPortCenter.y += (y / m_viewPortScale);

    updateViewRect();
}

void SGView::CameraScale(bool zoom)
{
    if (zoom)
    {
        m_viewPortScale *= 1.01f;
    }
    else
    {
        m_viewPortScale /= 1.01f;
    }

    updateViewRect();
}

void SGView::renderLater()
{
    requestUpdate();
}

bool SGView::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::UpdateRequest:
        render();
        return true;
    default:
        return QWindow::event(event);
    }
}

void SGView::resizeEvent(QResizeEvent * event)
{
    //m_projection
    updateViewRect();
}

void SGView::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);

    if (isExposed())
        render();
}

void SGView::updateViewRect()
{
    int vwid = this->width();
    int vhei = this->height();

    int swid = vwid / m_viewPortScale;
    int shei = vhei / m_viewPortScale;
    m_viewPortRect = Rect(0, 0, swid, shei);
    Point oldc = m_viewPortRect.center();
    m_viewPortRect.move(m_viewPortCenter - oldc);
    m_viewTransform = glm::ortho(float(m_viewPortRect.tl.x), float(m_viewPortRect.br.x), float(m_viewPortRect.br.y), float(m_viewPortRect.tl.y), -1.f, 1.f);
}

void SGView::render()
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

    if (needsInitialize) {
        m_render->initializeGL();
        m_render->sync(&m_scene->m_root);
    }

    m_render->render();

    m_context->swapBuffers(this);

    requestUpdate();
}
