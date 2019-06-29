#include "viewinteraction.h"
#include <QEvent>
#include <QKeyEvent>
#include <QTimer>
#include <QDebug>
#include "sgview.h"

ViewInteraction::ViewInteraction(SGView *parent) :
    QObject(parent),
    m_parent(parent),
    m_ptMousePosOld(-1, -1),
    m_b3DOperationState(false),
    m_bLeftMouseButtonPressed(false),
    m_bRightMouseButtonPressed(false),
    m_bCtrlPressed(false),
    m_bShiftPressed(false),
    m_iKeySpacePressedNum(0),
    m_ptMouseMoveDelta(-1, -1),
    m_bRectValid(false),
    m_bSelectedMoved(false),
    m_enumSelectMode(SELECT_NEW)
{
}

//按下空格键才会进行3d视图旋转等操作,此时选中等操作无效
bool ViewInteraction::eventFilter(QObject *obj, QEvent *event)
{
    QEvent::Type eventType = event->type();

    switch (eventType)
    {
    case QEvent::KeyPress:
        eatKeyPress(event);
        break;
    case QEvent::KeyRelease:
        eatKeyRelease(event);
        break;
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

void ViewInteraction::doRelativeScale()
{
    bool zoom = m_ptMouseMoveDelta.y() > 0;
    if (m_bCtrlPressed)
    { }
    if (m_bShiftPressed)
    { }
}

void ViewInteraction::eatKeyPress(QEvent * event)
{
    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
    if (keyEvent->key() == Qt::Key_Control)
    {
        //qDebug() << "Qt::Key_Control Press";
        m_bCtrlPressed = true;
        m_enumSelectMode = SELECT_ADD;
    }
    else if (keyEvent->key() == Qt::Key_Shift)
    {
        //qDebug() << "Qt::Key_Shift Press";
        m_bShiftPressed = true;
        m_enumSelectMode = SELECT_MINUS;
    }
    else
    {
        eatKeyPressHelper(keyEvent->key());
    }
}

void ViewInteraction::eatKeyPressHelper(int key)
{
    if (key == Qt::Key_Space)
    {
        m_iKeySpacePressedNum++;
        if (1 == m_iKeySpacePressedNum)
        {
            QTimer::singleShot(200, this, SLOT(slotEatSpacePress()));
        }
    }
    else
    {

    }
}


void ViewInteraction::eatKeyRelease(QEvent * event)
{
    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
    if (keyEvent->key() == Qt::Key_Control)
    {
        //qDebug() << "Qt::Key_Control Release";
        m_bCtrlPressed = false;
        m_enumSelectMode = m_bShiftPressed ? SELECT_MINUS : SELECT_NEW;
    }
    else if (keyEvent->key() == Qt::Key_Shift)
    {
        //qDebug() << "Qt::Key_Shift Release";
        m_bShiftPressed = false;
        m_enumSelectMode = m_bCtrlPressed ? SELECT_ADD : SELECT_NEW;
    }
}

void ViewInteraction::eatMouseWheel(QEvent * event)
{
    QWheelEvent *wheelEvent = static_cast<QWheelEvent *>(event);
    m_ptMouseMoveDelta = wheelEvent->angleDelta();

    if (m_bCtrlPressed || m_bShiftPressed)
    {
        doRelativeScale();
    }
    else
    {
        m_parent->CameraScale(m_ptMouseMoveDelta.y() > 0);
    }
}

void ViewInteraction::eatMousePress(QEvent * event)
{
    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    //qDebug() <<"eatMousePress"<< mouseEvent->button();
    QPoint pos = mouseEvent->pos();
    if (mouseEvent->button() == Qt::MouseButton::LeftButton)
    {
        //qDebug()<<mouseEvent->pos();
        m_startPos = pos;

        m_bLeftMouseButtonPressed = true;
        m_bRightMouseButtonPressed = false;
            
        m_ptMousePosOld = m_startPos;
        //if(!m_b3DOperationState) m_bCatched = m_parent->catchedNode(m_startPos);
    }
    else if (mouseEvent->button() == Qt::MouseButton::RightButton)
    {
        m_bRightMouseButtonPressed = true;
        m_bLeftMouseButtonPressed = false;
            
        //m_parent->CheckRightButtonClicked(pos);
    }
}

void ViewInteraction::eatMouseRelease(QEvent * event)
{

    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    //qDebug() <<"eatMouseRelease"<< mouseEvent->button();
    if (mouseEvent->button() == Qt::MouseButton::LeftButton)
    {
        if (m_bRectValid)
        {
            //m_parent->UpdateSelect(m_rectSelect, m_enumSelectMode);//选中计算
            //m_parent->ShowFrameSelect(false);//隐藏选中框
            m_bRectValid = false;
        }
        else if (!m_b3DOperationState && !m_bSelectedMoved)
        {
            //m_parent->UpdateSelect(mouseEvent->pos(), m_enumSelectMode);
        }
        m_bLeftMouseButtonPressed = false;
        m_bCatched = false;
    }
    else if (mouseEvent->button() == Qt::MouseButton::RightButton)
        m_bRightMouseButtonPressed = false;

    m_ptMousePosOld.setX(-1);
    m_bSelectedMoved = false;
}

void ViewInteraction::eatMouseMove(QEvent * event)
{
    QPoint mousePos = static_cast<QMouseEvent *>(event)->pos();
    if (!m_b3DOperationState)
    {
        if (m_bCatched)
        {
            //拖动
            QPoint deltaPos = mousePos - m_ptMousePosOld;
            //m_parent->MoveSelect(deltaPos);
            m_bSelectedMoved = true;
        }
        else
        {
            //框选
            moveWithSelectMode(mousePos);
        }
    }
    else
    {
        moveWithCameraMode(mousePos);
    }

    m_ptMousePosOld = mousePos;
}

void ViewInteraction::moveWithSelectMode(const QPoint &mousePos)
{
    if (m_bLeftMouseButtonPressed)
    {
        m_endPos = mousePos;
        //m_rectSelect = AlgHelper::createRect(m_startPos, m_endPos);
        m_bRectValid = true;
        //m_parent->ShowFrameSelect(true, m_rectSelect);
    }
    else
    {
        m_bRectValid = false;
    }
}

void ViewInteraction::moveWithCameraMode(const QPoint &mousePos)
{
    if (!m_bLeftMouseButtonPressed && !m_bRightMouseButtonPressed)
        return;

    m_ptMouseMoveDelta = mousePos - m_ptMousePosOld;

    if (m_ptMousePosOld.x() > 0)
    {
        m_bLeftMouseButtonPressed ?
            m_parent->CameraMove(-m_ptMouseMoveDelta.x(), -m_ptMouseMoveDelta.y()) :nullptr;
    }
}

void ViewInteraction::slotEatSpacePress()
{
    if (m_iKeySpacePressedNum == 1)
    {
        m_b3DOperationState = !m_b3DOperationState;
        if (m_b3DOperationState)
            m_parent->setCursor(Qt::OpenHandCursor);
        else
            m_parent->unsetCursor();
    }
    else if (m_iKeySpacePressedNum == 2)
    {
        //m_parent->CameraReset();
    }
    m_iKeySpacePressedNum = 0;
}