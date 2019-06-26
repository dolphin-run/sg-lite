#ifndef SGVIEW_H
#define SGVIEW_H

#include <QWindow>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

class SGNode;
class SGScene;
class SGRenderer;
class SGFlatColorMaterial;
class SGView : public QWindow
{
    Q_OBJECT
public:
    explicit SGView(QWindow *parent = 0);
    ~SGView();

    void setScene(SGScene *scene);
    virtual void render();

    virtual void initialize();

public slots:
    void renderLater();

protected:
    bool event(QEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

    void exposeEvent(QExposeEvent *event) override;

    //void drawItem(SGNode *node, SGFlatColorMaterial* mt);

private:
    QOpenGLContext *m_context;

    SGScene *m_scene = nullptr;
    SGRenderer *m_render = nullptr;
};

#endif // SGVIEW_H
