#include "view.h"
#include "sgviewcamera.h"
#include <QOpenGLContext>
#include <QDateTime>

static QOpenGLContext *g_context = nullptr;


View::View(QWidget *parent)
    : QOpenGLWidget(parent)
    , m_context(0)
{
    setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    m_pUserInteraction = new ViewInteraction(this);
    this->installEventFilter(m_pUserInteraction);

    connect(m_pUserInteraction, SIGNAL(signalScale(bool)), this, SLOT(slotCameraScale(bool)));
    connect(m_pUserInteraction, SIGNAL(signalMove(int, int)), this, SLOT(slotCameraMove(int, int)));

    m_fpsLabel = new QLabel(this);
    m_fpsLabel->setFixedSize(60, 30);
    m_fpsLabel->setStyleSheet("background-color: green; color: white; qproperty-alignment: AlignCenter;");
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
    int radio = devicePixelRatio();
    SGView::setSize(w, h);
    SGView::setSizePx(w*radio, h*radio);
}

void View::paintGL()
{
    calculateFps();
    if (!isVisible())
        return;
    SGView::renderFrame();

    m_fpsLabel->setText(QString::asprintf("%d", m_currentFps));

    update();
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

void View::calculateFps()
{
    qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    m_times.push_back(currentTime);

    while (m_times[0] < currentTime - 1000) {
        m_times.pop_front();
    }

    int currentCount = m_times.length();
    m_currentFps = (currentCount + m_cacheCount) / 2;

    m_cacheCount = currentCount;
}
