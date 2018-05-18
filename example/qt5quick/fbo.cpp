#include "fbo.h"
#include "fborenderer.h"
#include <QTimer>

SceneQuickItem::SceneQuickItem(QQuickItem *parent) :
    QQuickFramebufferObject(parent),
    m_background("black")
{
    QTimer *tm = new QTimer(this);
    tm->setTimerType(Qt::TimerType::PreciseTimer);
    connect(tm, SIGNAL(timeout()), this, SLOT(update()));
    tm->start(20);
}

SceneQuickItem::~SceneQuickItem()
{
}

QColor SceneQuickItem::background()
{
    return m_background;
}

void SceneQuickItem::setBackground(QColor color)
{
    m_background = color;
}

QQuickFramebufferObject::Renderer * SceneQuickItem::createRenderer() const
{
    auto ptr = new SceneQuickItemRenderer;

    return ptr;
}
