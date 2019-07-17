#include "sgview.h"
#include "sgscene.h"
#include "sgrenderer.h"

//camera
void ViewCamera::setSceneSize(int wid, int hei)
{
    m_swid = wid;
    m_shei = hei;
    update();
}

void ViewCamera::setViewSize(int wid, int hei)
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

void ViewCamera::move(int x, int y)
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
    float wid = m_vwid / m_scale;
    float hei = m_vhei / m_scale;
    m_viewPort = RectF(0, 0, wid, hei);
    PointF oldcenter = m_viewPort.center();
    m_viewPort.move(m_center - oldcenter);
    m_transform = glm::ortho(m_viewPort.tl.x, m_viewPort.br.x, m_viewPort.br.y, m_viewPort.tl.y, -1.f, 1.f);
}

//SGView
SGView::SGView()
{
    m_render = new SGRenderer(this);
}

SGView::~SGView()
{
    if (m_scene) detach();
}

void SGView::attach(SGScene *scene)
{
    if (m_scene) detach();

    m_scene = scene;
    sceneChanged();

    //notify view' renderer
    m_scene->addView(this);
}

void SGView::detach()
{
    m_scene->m_views.remove(this);
    m_scene = nullptr;
    m_render->setRootNode(nullptr);
    m_render->clear();
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