#ifndef VIEW_H
#define VIEW_H

#include <QOpenGLWidget>
#include <QLabel>
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
    void calculateFps();
private:
    QOpenGLContext *m_context; 
    int m_currentFps;
    int m_cacheCount;
    QVector<qint64> m_times;
    QLabel *m_fpsLabel;

    ViewInteraction *m_pUserInteraction;
};

#endif // VIEW_H
