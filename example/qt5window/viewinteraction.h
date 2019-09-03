
#ifndef VIEWINTERACTION_H
#define VIEWINTERACTION_H

#include <QObject>
#include <QPoint>

class ViewInteraction : public QObject
{
    Q_OBJECT
public:
    ViewInteraction(QObject *parent);

signals:
    void signalScale(bool zoom);
    void signalMove(int x, int y);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

    void eatMouseWheel(QEvent *event);
    void eatMousePress(QEvent *event);
    void eatMouseRelease(QEvent *event);
    void eatMouseMove(QEvent *event);

protected:
    bool m_bLeftMouseButtonPressed;
    bool m_bRightMouseButtonPressed;

    QPoint m_oldPos;
    QPoint m_startPos;
    QPoint m_endPos;
};

#endif // VIEWINTERACTION_H
