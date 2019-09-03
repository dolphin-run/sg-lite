#ifndef VIEW_H
#define VIEW_H

#include <QOpenGLWidget>
#include "sgview.h"


class View : public QOpenGLWidget, public SGView
{
    Q_OBJECT
public:
    explicit View(QWidget *parent = 0);
    ~View();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
};

#endif // VIEW_H
