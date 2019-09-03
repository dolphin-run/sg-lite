#ifndef VIEW_H
#define VIEW_H

#include <QOpenGLWidget>
#include "sgview.h"
#include "viewinteraction.h"


class View : public QOpenGLWidget, public SGView
{
    Q_OBJECT
public:
    explicit View(QWidget *parent = 0);
    ~View();

    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

public slots:
    void slotCameraMove(int x, int y);
    void slotCameraScale(bool zoom);

private:
    QOpenGLContext *m_context;

    ViewInteraction *m_pUserInteraction;
};

#endif // VIEW_H
