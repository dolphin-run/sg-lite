#ifndef SGVIEW_H
#define SGVIEW_H

#include <QWindow>
#include "sgstruct.h"
#include "viewinteraction.h"

class SGNode;
class SGScene;
class SGRenderer;
class SGView : public QWindow
{
    Q_OBJECT
public:
    explicit SGView(QWindow *parent = 0);
    ~SGView();

    void setScene(SGScene *scene);
    const Rect &viewRect() const;
    const QMatrix4x4 &transform() const;
    void CameraMove(int x, int y);
    void CameraScale(bool zoom);

    virtual void initialize();

public slots:
    void renderLater();

protected:
    void updateViewRect();

protected:
    virtual void render();

    virtual bool event(QEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void exposeEvent(QExposeEvent *event) override;

private:
    QOpenGLContext *m_context;

    SGScene *m_scene = nullptr;
    SGRenderer *m_render = nullptr;

    Point m_viewPortCenter;
    float m_viewPortScale = 1.0;
    Rect m_viewPortRect;//dummy by view port's center&scale and viewWidth&viewHeight

    QMatrix4x4 m_viewTransform;
    ViewInteraction *m_pUserInteraction;
};

#endif // SGVIEW_H
