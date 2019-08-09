#include "sgview.h"
#include "sgscene.h"
#include "sgrenderer.h"

//camera
void ViewCamera::setSceneSize(float wid, float hei)
{
    m_swid = wid;
    m_shei = hei;
    update();
}

void ViewCamera::setViewSize(float wid, float hei)
{
    m_vwid = wid;
    m_vhei = hei;
    update();
}

void ViewCamera::moveCenter()
{
    m_center.x = m_swid / 2;
    m_center.y = m_shei / 2;
    update();
}

void ViewCamera::move(float x, float y)
{
    m_center.x += (x / m_scale);
    m_center.y += (y / m_scale);
    update();
}

void ViewCamera::scale(float s)
{
    m_scale *= s;

    update();
}

void ViewCamera::setFillMode(bool strench)
{
    m_stretch = strench;
}

const glm::mat4 &ViewCamera::transform() const
{
    return m_transform;
}

const RectF &ViewCamera::viewRect() const
{
    return m_viewPort;
}

Point ViewCamera::mapToScene(const Point & vp) const
{
    Point pt;
    pt.x = m_viewPort.tl.x + vp.x / m_scale;
    pt.y = m_viewPort.tl.y + vp.y / m_scale;

    return pt;
}

void ViewCamera::update()
{
    if (m_shei < 1.f || m_swid < 1.f || m_vwid < 1.f || m_vhei < 1.f) return;
    
    if (!m_stretch)
    {
        float wid = m_vwid / m_scale;
        float hei = m_vhei / m_scale;
        m_viewPort = RectF(0, 0, wid, hei);
        PointF oldcenter = m_viewPort.center();
        m_viewPort.move(m_center - oldcenter);
        m_transform = glm::ortho(m_viewPort.tl.x, m_viewPort.br.x, m_viewPort.br.y, m_viewPort.tl.y, -1.f, 1.f);
        //m_transform = glm::ortho(m_viewPort.tl.x, m_viewPort.br.x, m_viewPort.tl.y, m_viewPort.br.y, -1.f, 1.f);
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
        m_transform = glm::ortho(deltawid, actwid + deltawid, acthei + deltahei, deltahei, -1.f, 1.f);
        //m_transform = glm::ortho(deltawid, actwid + deltawid, deltahei, acthei + deltahei, -1.f, 1.f);
    }
}

//SGView
SGView::SGView()
{
    m_render = new SGRenderer(this);
}

SGView::~SGView()
{
    detach();
}

void SGView::attach(SGScene *scene)
{
    detach();

    m_scene = scene;
    sceneChanged();

    //notify view' renderer
    m_scene->addView(this);
}

void SGView::detach()
{
    if (m_scene)
    {
        m_scene->delView(this);

        m_scene = nullptr;
        m_render->setRootNode(nullptr);
        m_render->clear();
    }
}

void SGView::setViewMode(bool strench)
{
    m_camera.setFillMode(strench);
}

void SGView::sceneChanged()
{
    if (!m_scene) return;

    m_render->setRootNode(&m_scene->m_root);
    sceneSizeChanged();
}

void SGView::sceneSizeChanged()
{
    if (!m_scene) return;

    m_camera.setSceneSize(m_scene->m_width, m_scene->m_height);
}

void SGView::itemAdded(SGNode *item)
{
    m_render->itemAdded(item);
}

void SGView::itemDeleted(SGNode *item)
{
    m_render->itemDeleted(item);
}

void SGView::itemChanged(SGNode *item, SGNode::E_DirtyType changes)
{
    m_render->itemChanged(item, changes);
}

const RectF & SGView::sceneRect() const
{
    return m_camera.viewRect();
}

const glm::mat4 & SGView::sceneTransform() const
{
    return m_camera.transform();
}

Point SGView::mapToScene(const Point & vp) const
{
    return m_camera.mapToScene(vp);
}

bool SGView::initializeRender()
{
    if (!m_scene) return false;

    m_camera.moveCenter();

    return m_render->initialize();
}

void SGView::renderFrame()
{
    if (!m_renderInited)
    {
        m_renderInited = initializeRender();
    }
    m_render->render();
}