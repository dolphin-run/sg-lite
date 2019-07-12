#ifndef SGVIEW_H
#define SGVIEW_H

#include "sgstruct.h"

class ViewCamera
{
public:
    void setSceneSize(int wid, int hei);
    void setViewSize(int wid, int hei);
    void moveCenter();
    void move(int x, int y);
    void scale(float s);
    const glm::mat4 &transform() const;
    const Rect &viewRect() const;

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
    //bool stretch = false;
};

class SGScene;
class SGRenderer;
class SGView
{
public:
    explicit SGView();
    ~SGView();

    void attach(SGScene *scene);
    void detach();
    void sceneChanged();

    const Rect &sceneRect() const;
    const glm::mat4 &sceneTransform() const;

    virtual int width() const = 0;
    virtual int height() const = 0;
    virtual float pixelRatio() const = 0;

protected:
    virtual void initialize();
    virtual void renderFrame();

protected:
    ViewCamera m_camera;

private:
    SGScene *m_scene = nullptr;
    SGRenderer *m_render = nullptr;
};

#endif // SGVIEW_H
