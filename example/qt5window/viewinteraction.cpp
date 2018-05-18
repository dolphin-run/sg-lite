#include "viewinteraction.h"
#include <QEvent>
#include <QKeyEvent>
#include "view.h"

ViewInteraction::ViewInteraction(QObject *parent) :
    QObject(parent),
    m_bLeftMouseButtonPressed(false),
    m_bRightMouseButtonPressed(false)
{
}

bool ViewInteraction::eventFilter(QObject *obj, QEvent *event)
{
    QEvent::Type eventType = event->type();

    switch (eventType)
    {
    case QEvent::Wheel:
        eatMouseWheel(event);
        break;
    case QEvent::MouseButtonPress:
        eatMousePress(event);
        break;
    case QEvent::MouseButtonRelease:
        eatMouseRelease(event);
        break;
    case QEvent::MouseMove:
        eatMouseMove(event);
        break;
    default:
        break;
    }
    return QObject::eventFilter(obj, event);
}

void ViewInteraction::eatMouseWheel(QEvent * event)
{
    QWheelEvent *wheelEvent = static_cast<QWheelEvent *>(event);
    auto delta = wheelEvent->angleDelta();

    emit signalScale(delta.y() > 0);
}

void ViewInteraction::eatMousePress(QEvent * event)
{
    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    if (mouseEvent->button() == Qt::MouseButton::LeftButton)
    {
        m_startPos = mouseEvent->pos();
        m_oldPos = m_startPos;
        m_bLeftMouseButtonPressed = true;
    }
}

void ViewInteraction::eatMouseRelease(QEvent * event)
{
    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    if (mouseEvent->button() == Qt::MouseButton::LeftButton)
    {
        m_bLeftMouseButtonPressed = false;
    }
}

void ViewInteraction::eatMouseMove(QEvent * event)
{
    QPoint mousePos = static_cast<QMouseEvent *>(event)->pos();
    if (m_bLeftMouseButtonPressed)
    {
        auto delta = mousePos - m_oldPos;

        emit signalMove(-delta.x(), -delta.y());
    }

    m_oldPos = mousePos;
}