#include "view.h"
#include "sgviewcamera.h"
#include <QOpenGLContext>
#include <QMouseEvent>
#include "node/sgrectanglenode.h"
#include "sgscene.h"

static QOpenGLContext *g_context = nullptr;

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
    case QEvent::Close:
        printf("close");
        this->deleteLater();
    default:
        return QWindow::event(event);
    }
}

void View::resizeEvent(QResizeEvent * event)
{
    SGView::setSize(this->width(), this->height());
    SGView::setSizePx(this->width()*QWindow::devicePixelRatio(), this->height()*QWindow::devicePixelRatio());
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

    if (!m_context) {
        m_context = new QOpenGLContext(this);
        m_context->setFormat(requestedFormat());
        if (g_context)
        {
            m_context->setShareContext(g_context);
        }
        else
        {
            g_context = m_context;
        }
        m_context->create();
    }

    m_context->makeCurrent(this);

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

    connect(m_pUserInteraction, SIGNAL(signalScale(bool)), this, SLOT(slotCameraScale(bool)));
    connect(m_pUserInteraction, SIGNAL(signalMove(int, int)), this, SLOT(slotCameraMove(int, int)));
}


View::~View()
{
}

void View::slotCameraMove(int x, int y)
{
    this->viewCamera()->move(x, y);
}

void View::slotCameraScale(bool zoom)
{
    if (zoom)
    {
        this->viewCamera()->scale(1.01f);
    }
    else
    {
        this->viewCamera()->scale(1.f / 1.01f);
    }
}
