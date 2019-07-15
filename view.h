#ifndef VIEW_H
#define VIEW_H

#include <QWindow>
#include "sgview.h"
#include "viewinteraction.h"


class View : public QWindow, public SGView
{
    Q_OBJECT
public:
    explicit View(QWindow *parent = 0);
    ~View();


    virtual int width() const override;
    virtual int height() const override;
    virtual float pixelRatio() const override;

    void CameraMove(int x, int y);
    void CameraScale(bool zoom);

public slots:
    void renderLater();

protected:
    void render();
    virtual void mousePressEvent(QMouseEvent *ev) override;


protected:
    virtual bool event(QEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void exposeEvent(QExposeEvent *event) override;

private:
    QOpenGLContext *m_context;

    ViewInteraction *m_pUserInteraction;
};

#endif // VIEW_H
