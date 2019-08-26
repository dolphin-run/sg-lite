#ifndef SGVIEW_H
#define SGVIEW_H

#include "sgstruct.h"
#include "sgnode.h"

class SGScene;
class SGRenderer;
class SGViewCamera;

/*
** subclass should supply OpenGL Render Context as needed.
*/
class SGView
{
public:
    explicit SGView();
    virtual ~SGView();

    void attach(SGScene *scene);
    void detach();

    void setViewMode(bool strench);
    void setViewCamera(SGViewCamera *camera);

    //notify renderer rebuild.
    virtual void sceneChanged();
    virtual void sceneSizeChanged();
    virtual void itemAdded(SGNode *item);
    virtual void itemDeleted(SGNode *item);
    virtual void itemChanged(SGNode *item, SGNode::E_DirtyType changes);

    //notify view size changed.
    virtual void sizeChanged(int width, int height);

    const RectF &sceneRect() const;
    const glm::mat4 &sceneTransform() const;

    Point mapToScene(const Point &vp) const;

    virtual float widthPx() const = 0;
    virtual float heightPx() const = 0;

protected:
    bool m_renderInited = false;
    virtual bool initializeRender();
    virtual void renderFrame();

protected:
    SGViewCamera *m_camera = nullptr;
    SGScene *m_scene = nullptr;
    SGRenderer *m_render = nullptr;

    int m_width;
    int m_height;
};

#endif // SGVIEW_H
