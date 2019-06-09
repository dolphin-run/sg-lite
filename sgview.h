#ifndef SGVIEW_H
#define SGVIEW_H

#include <QWindow>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

class SGNode;
class SGScene;
class SGFlatColorMaterial;
class SGView : public QWindow, protected QOpenGLFunctions
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
    void renderNow();

protected:
    bool event(QEvent *event) override;

    void exposeEvent(QExposeEvent *event) override;

    void drawItem(SGNode *node, SGFlatColorMaterial* mt);

private:
    QOpenGLContext *m_context;

    SGScene *m_scene=nullptr;
};

#endif // SGVIEW_H
