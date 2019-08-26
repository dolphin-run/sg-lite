#include "sgviewcamera.h"

//camera
void SGViewCamera::setSceneSize(float wid, float hei)
{
    m_swid = wid;
    m_shei = hei;
    update();
}

void SGViewCamera::setViewSize(float wid, float hei)
{
    m_vwid = wid;
    m_vhei = hei;
    update();
}

void SGViewCamera::moveCenter()
{
    m_center.x = m_swid / 2;
    m_center.y = m_shei / 2;
    update();
}

void SGViewCamera::move(float x, float y)
{
    m_center.x += (x / m_scale);
    m_center.y += (y / m_scale);
    update();
}

void SGViewCamera::scale(float s)
{
    m_scale *= s;

    update();
}

void SGViewCamera::setFillMode(bool strench)
{
    m_stretch = strench;
}

const glm::mat4 &SGViewCamera::transform() const
{
    return m_transform;
}

const RectF &SGViewCamera::viewRect() const
{
    return m_viewPort;
}

void SGViewCamera::update()
{
    if (m_shei < 1.f || m_swid < 1.f || m_vwid < 1.f || m_vhei < 1.f) return;
    
    if (!m_stretch)
    {
        float wid = m_vwid / m_scale;
        float hei = m_vhei / m_scale;
        m_viewPort = RectF(0, 0, wid, hei);
        PointF oldcenter = m_viewPort.center();
        m_viewPort.move(m_center - oldcenter);

#ifdef USERVIEW_HAS_VERTICAL_MIRROR
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

#ifdef USERVIEW_HAS_VERTICAL_MIRROR
        m_transform = glm::ortho(deltawid, actwid + deltawid, deltahei, acthei + deltahei, -1.f, 1.f);
#else
        m_transform = glm::ortho(deltawid, actwid + deltawid, acthei + deltahei, deltahei, -1.f, 1.f);
#endif
    }
}