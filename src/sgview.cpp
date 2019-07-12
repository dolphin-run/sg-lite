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

const Rect &ViewCamera::viewRect() const
{
    return m_viewPort.toRect();
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
}

void SGView::attach(SGScene *scene)
{
    if (m_scene) detach();

    m_scene = scene;
    m_scene->m_views.push_back(this); 

    sceneChanged();
}

void SGView::detach()
{
    m_scene->m_views.remove(this);
    m_scene = nullptr;
}

void SGView::sceneChanged()
{
    m_camera.setSceneSize(m_scene->m_width, m_scene->m_height);
}

const Rect & SGView::sceneRect() const
{
    return m_camera.viewRect();
}

const glm::mat4 & SGView::sceneTransform() const
{
    return m_camera.transform();
}

void SGView::initialize()
{
    m_camera.moveCenter();

    m_render->initializeGL();
    m_render->sync(&m_scene->m_root);
}

void SGView::renderFrame()
{
    m_render->render();
}