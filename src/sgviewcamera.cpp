#include "sgviewcamera.h"
#include "sgviewcamera_p.h"


SGViewCamera::SGViewCamera()
{
    SG_INIT_PRIVATE(new SGViewCameraPrivate);
}

SGViewCamera::~SGViewCamera()
{
    SG_FREE_PRIVATE();
}

//camera
void SGViewCamera::setSceneSize(float wid, float hei)
{
    d_ptr->m_swid = wid;
    d_ptr->m_shei = hei;
    update();
}

void SGViewCamera::setViewSize(float wid, float hei)
{
    d_ptr->m_vwid = wid;
    d_ptr->m_vhei = hei;
    update();
}

void SGViewCamera::moveCenter()
{
    d_ptr->m_center.x = d_ptr->m_swid / 2;
    d_ptr->m_center.y = d_ptr->m_shei / 2;
    update();
}

void SGViewCamera::move(float x, float y)
{
    d_ptr->m_center.x += (x / d_ptr->m_scale);
    d_ptr->m_center.y += (y / d_ptr->m_scale);
    update();
}

void SGViewCamera::scale(float s)
{
    d_ptr->m_scale *= s;

    update();
}

void SGViewCamera::setFillMode(bool strench)
{
    d_ptr->m_stretch = strench;
}

void SGViewCamera::reset()
{
}

const float* SGViewCamera::transform() const
{
    return &d_ptr->m_transform[0][0];
}

const RectF &SGViewCamera::viewRect() const
{
    return d_ptr->m_viewPort;
}

Point SGViewCamera::mapToScene(const Point & vp) const
{
    Point pt;
    pt.x = d_ptr->m_viewPort.tl.x + vp.x / d_ptr->m_scale;
    pt.y = d_ptr->m_viewPort.tl.y + vp.y / d_ptr->m_scale;

    return pt;
}

void SGViewCamera::update()
{
    d_ptr->updateMatrix();
}

void SGViewCameraPrivate::updateMatrix()
{
    if (m_shei < 1.f || m_swid < 1.f || m_vwid < 1.f || m_vhei < 1.f) return;
    
    if (!m_stretch)
    {
        float wid = m_vwid / m_scale;
        float hei = m_vhei / m_scale;
        m_viewPort = RectF(0, 0, wid, hei);
        PointF oldcenter = m_viewPort.center();
        m_viewPort.move(m_center - oldcenter);

#ifdef SG_VIEW_VERTICAL_MIRROR
        m_transform = glm::ortho(m_viewPort.tl.x, m_viewPort.br.x, m_viewPort.tl.y, m_viewPort.br.y, -1.f, 1.f);
#else
        m_transform = glm::ortho(m_viewPort.tl.x, m_viewPort.br.x, m_viewPort.br.y, m_viewPort.tl.y, -1.f, 1.f);
#endif
    }
    else 
    {
        float actwid = m_swid, acthei = m_shei;
        float vradio = m_vhei / m_vwid;
        float sradio = m_shei / m_swid;
        float deltawid = 0.f, deltahei = 0.f;
        if (vradio > sradio)
        {
            //view with more height
            acthei = actwid*vradio;
            deltahei = (m_shei - acthei) / 2.f;
        }
        else
        {
            actwid = acthei / vradio;
            deltawid = (m_swid - actwid) / 2.f;
        }

#ifdef SG_VIEW_VERTICAL_MIRROR
        m_transform = glm::ortho(deltawid, actwid + deltawid, deltahei, acthei + deltahei, -1.f, 1.f);
#else
        m_transform = glm::ortho(deltawid, actwid + deltawid, acthei + deltahei, deltahei, -1.f, 1.f);
#endif
    }
}