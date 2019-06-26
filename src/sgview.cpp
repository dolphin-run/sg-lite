#include "sgview.h"
#include <QScreen>
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
}

void SGView::setScene(SGScene *scene)
{
    m_scene = scene;
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
}

void SGView::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);

    if (isExposed())
        render();
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
    }

    m_render->render(&m_scene->m_root);

    m_context->swapBuffers(this);
}
