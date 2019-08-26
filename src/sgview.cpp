#include "sgview.h"
#include "sgscene.h"
#include "sgrenderer.h"
#include "sgviewcamera.h"

//SGView
SGView::SGView()
{
    m_render = new SGRenderer(this);
    m_camera = new SGViewCamera;
}

SGView::~SGView()
{
    detach();
    delete m_camera;
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
    m_camera->setFillMode(strench);
}

void SGView::setViewCamera(SGViewCamera * camera)
{
    m_camera = camera;

    //update camera changed...
    sceneSizeChanged();
    sizeChanged(m_width, m_height);
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

    m_camera->setSceneSize(m_scene->m_width, m_scene->m_height);
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

void SGView::sizeChanged(int width, int height)
{
    m_width = width;
    m_height = height;
    m_camera->setViewSize(width, height);
}

const RectF & SGView::sceneRect() const
{
    return m_camera->viewRect();
}

const glm::mat4 & SGView::sceneTransform() const
{
    return m_camera->transform();
}

Point SGView::mapToScene(const Point & vp) const
{
    Point pt;
    pt.x = m_camera->m_viewPort.tl.x + vp.x / m_camera->m_scale;
    pt.y = m_camera->m_viewPort.tl.y + vp.y / m_camera->m_scale;

    return pt;
}

bool SGView::initializeRender()
{
    if (!m_scene) return false;

    m_camera->moveCenter();
    m_renderInited = m_render->initialize();

    return m_renderInited;
}

void SGView::renderFrame()
{
    if (!m_renderInited)
    {
        m_renderInited = initializeRender();
    }
    else
    {
        m_render->render();
    }
}