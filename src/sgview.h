#ifndef SGVIEW_H
#define SGVIEW_H

#include "sgstruct.h"
#include "sgnode.h"

class ViewCamera
{
public:
    void setSceneSize(float wid, float hei);
    void setViewSize(float wid, float hei);
    void moveCenter();
    void move(float x, float y);
    void scale(float s);
    void setFillMode(bool strench);

    const glm::mat4 &transform() const;
    const RectF &viewRect() const;

    Point mapToScene(const Point &vp) const;

protected:
    void update();
private:
    glm::mat4 m_transform;
    //
    float m_swid = 100;
    float m_shei = 100;
    float m_vwid = 100;
    float m_vhei = 100;

    PointF m_center;
    float m_scale = 1.0;
    RectF m_viewPort;
    bool m_stretch = false;//fill mode
};

class SGScene;
class SGRenderer;

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

    //notify renderer rebuild.
    virtual void sceneChanged();
    virtual void sceneSizeChanged();
    virtual void itemAdded(SGNode *item);
    virtual void itemDeleted(SGNode *item);
    virtual void itemChanged(SGNode *item, SGNode::E_DirtyType changes);

    const RectF &sceneRect() const;
    const glm::mat4 &sceneTransform() const;

    Point mapToScene(const Point &vp) const;

    virtual float width() const = 0;
    virtual float height() const = 0;
    virtual float widthPx() const = 0;
    virtual float heightPx() const = 0;
    virtual float pixelRatio() const = 0;

protected:
    bool m_renderInited = false;
    virtual bool initializeRender();
    virtual void renderFrame();

protected:
    ViewCamera m_camera;
    SGScene *m_scene = nullptr;
    SGRenderer *m_render = nullptr;
};

#endif // SGVIEW_H
