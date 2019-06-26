#ifndef SGVIEW_H
#define SGVIEW_H

#include <QWindow>

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

    virtual void initialize();

public slots:
    void renderLater();

protected:
    virtual void render();

    virtual bool event(QEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void exposeEvent(QExposeEvent *event) override;

private:
    QOpenGLContext *m_context;

    SGScene *m_scene = nullptr;
    SGRenderer *m_render = nullptr;
};

#endif // SGVIEW_H
