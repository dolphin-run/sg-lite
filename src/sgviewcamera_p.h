#ifndef SGVIEWCAMERA_P_H
#define SGVIEWCAMERA_P_H

#include "sgstruct.h"
#include "sgglobal.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class SGViewCameraPrivate
{
    SG_DECLARE_PUBLIC(SGViewCamera);
public:
    void updateMatrix();

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
    bool m_horMirrored = false;
    bool m_verMirrored = false;

    SGViewCamera *q_ptr;
};

#endif // SGVIEWCAMERA_P_H
