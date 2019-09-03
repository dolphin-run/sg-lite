#ifndef VIEW_H
#define VIEW_H

#include <QWindow>
#include "sgview.h"
#include "../qt5widget/viewinteraction.h"


class View : public QWindow, public SGView
{
    Q_OBJECT
public:
    explicit View(QWindow *parent = 0);
    ~View();

public slots:
    void slotCameraMove(int x, int y);
    void slotCameraScale(bool zoom);
    void renderLater();

protected:
    void render();

protected:
    virtual void mousePressEvent(QMouseEvent * ev);
    virtual bool event(QEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void exposeEvent(QExposeEvent *event) override;

private:
    QOpenGLContext *m_context;

    ViewInteraction *m_pUserInteraction;
};

#endif // VIEW_H
