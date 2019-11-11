#include "sgview.h"
#include "sgscene.h"
#include "sgrenderer.h"
#include "sgviewcamera.h"

//SGView
SGView::SGView()
{
    m_render = new SGRenderer(this);
    m_camera = new SGViewCamera;//default camera
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

SGViewCamera* SGView::setViewCamera(SGViewCamera * camera)
{
    SGViewCamera * old = m_camera;

    m_camera = camera;
    cameraChanged();

    return old;
}

SGViewCamera * SGView::viewCamera()
{
    return m_camera;
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

void SGView::itemsCleaned()
{
    m_render->clear();
}

void SGView::setSize(int width, int height)
{
    m_width = width;
    m_height = height;

    sizeChanged();
}

void SGView::setSizePx(int wpx, int hpx)
{
    m_widthPx = wpx;
    m_heightPx = hpx;

    sizePxChanged();
}

void SGView::sceneChanged()
{
    if (!m_scene) return;

    m_render->setRootNode(m_scene->rootNode());

    if (m_camera)
    {
        m_camera->setSceneSize(m_scene->width(), m_scene->height());
    }
}

void SGView::sizeChanged()
{
    if (m_camera)
    {
        m_camera->setViewSize(m_width, m_height);
    }
}

void SGView::sizePxChanged()
{
}

void SGView::cameraChanged()
{
    if (!m_scene) return;

    m_camera->setViewSize(m_width, m_height);
    m_camera->setSceneSize(m_scene->width(), m_scene->height());
}

const RectF & SGView::viewRect() const
{
    return m_camera->viewRect();
}

Point SGView::mapToScene(const Point & vp) const
{
    return m_camera->mapToScene(vp);
}

int SGView::widthPx() const
{
    return m_widthPx;
}

int SGView::heightPx() const
{
    return m_heightPx;
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