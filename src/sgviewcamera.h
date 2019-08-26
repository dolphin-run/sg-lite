#ifndef SGVIEWCAMERA_H
#define SGVIEWCAMERA_H

#include "sgstruct.h"

class SGViewCamera
{
    friend class SGView;
public:
    void moveCenter();
    void move(float x, float y);
    void scale(float s);
    void setFillMode(bool strench);

protected:
    void setSceneSize(float wid, float hei);
    void setViewSize(float wid, float hei);
    const RectF &viewRect() const;
    void update();

    const glm::mat4 &transform() const;
private:
    glm::mat4 m_transform;
    //
    float m_swid = 100.f;
    float m_shei = 100.f;
    float m_vwid = 100.f;
    float m_vhei = 100.f;

    PointF m_center;
    float m_scale = 1.f;
    RectF m_viewPort;
    bool m_stretch = false;//fill mode
};

#endif // SGVIEWCAMERA_H
