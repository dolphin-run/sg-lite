
#ifndef VIEWINTERACTION_H
#define VIEWINTERACTION_H

#include <QObject>
#include <QPoint>
#include <QRect>

enum E_SELECT_MODE { SELECT_NEW, SELECT_ADD, SELECT_MINUS };
class View;
class ViewInteraction : public QObject
{
    Q_OBJECT
public:
    ViewInteraction(View *parent);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

    void doRelativeScale();

    void eatKeyPress(QEvent *event);
    void eatKeyPressHelper(int key);
    void eatKeyRelease(QEvent *event);
    void eatMouseWheel(QEvent *event);
    void eatMousePress(QEvent *event);
    void eatMouseRelease(QEvent *event);
    void eatMouseMove(QEvent *event);

    void moveWithSelectMode(const QPoint &mousePos);
    void moveWithCameraMode(const QPoint &mousePos);

    protected slots:
    void slotEatSpacePress();

protected:
    View *m_parent;
    QPoint m_ptMousePosOld;


private:
    //用户交互相关
    bool m_b3DOperationState;//true代表进入视图操作模式，false不能操作，只能选中，通过空格键切换
    bool m_bLeftMouseButtonPressed;
    bool m_bRightMouseButtonPressed;
    bool m_bCtrlPressed;
    bool m_bShiftPressed;
    bool m_bCatched;//点选选中准备拖动
    int  m_iKeySpacePressedNum;
    QPoint m_ptMouseMoveDelta;
    int m_iMiddleMouseAngleDelta;

    QPoint m_startPos;
    QPoint m_endPos;
    QRect m_rectSelect;
    bool m_bRectValid;
    bool m_bSelectedMoved;
    E_SELECT_MODE m_enumSelectMode;
};

#endif // VIEWINTERACTION_H
