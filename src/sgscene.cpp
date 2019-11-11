#include "sgscene.h"
#include "sgscene_p.h"
#include "sgnode_p.h"
#include "sgview.h"
#include <algorithm>

SGScene::SGScene()
{
    SG_INIT_PRIVATE(new SGScenePrivate);

    //initialize root node.
    d_ptr->m_root.d_ptr->m_scene = this;
}

SGScene::~SGScene()
{
    //detach will remove view from m_views, for safety, make a copy.
    auto views = d_ptr->m_views;
    for (auto view : views)
    {
        view->detach();
    }

    SG_FREE_PRIVATE();
}

void SGScene::sizeChanged()
{
    for (auto view : d_ptr->m_views)
    {
        view->sceneChanged();
    }
}

void SGScene::itemAdded(SGNode *item)
{
    for (auto view : d_ptr->m_views)
    {
        view->itemAdded(item);
    }
}

void SGScene::itemDeleted(SGNode *item)
{
    for (auto view : d_ptr->m_views)
    {
        view->itemDeleted(item);
    }
}

void SGScene::itemChanged(SGNode *item, SGNode::E_DirtyType changes)
{
    for (auto view : d_ptr->m_views)
    {
        view->itemChanged(item, changes);
    }
}

void SGScene::itemsCleaned()
{
    for (auto view : d_ptr->m_views)
    {
        view->itemsCleaned();
    }
}

bool SGScene::addItem(SGNode * item, SGNode *attached)
{
    if (d_ptr->m_nodes.end() != std::find(d_ptr->m_nodes.begin(), d_ptr->m_nodes.end(), item)) return false;

    if (!attached)
    {
        d_ptr->m_root.addChild(item);
    }
    else
    {
        assert(attached->d_ptr->m_scene == this);
        attached->addChild(item);
    }
    add(item);

    return true;
}

bool SGScene::removeItem(SGNode * item)
{
    if (d_ptr->m_nodes.end() != std::find(d_ptr->m_nodes.begin(), d_ptr->m_nodes.end(), item))
    {
        if (item->d_ptr->m_parent)
        {
            item->d_ptr->m_parent->removeChild(item);
        }
        remove(item);

        return true;
    }
    return false;
    
}

void SGScene::clear()
{
    for (auto itm : d_ptr->m_nodes)
    {
        delete itm;
    }
    d_ptr->m_nodes.clear();

    itemsCleaned();
}

void SGScene::setSize(int wid, int hei)
{
    d_ptr->m_width = wid;
    d_ptr->m_height = hei;

    sizeChanged();
}

float SGScene::width() const
{
    return d_ptr->m_width;
}

float SGScene::height() const
{
    return d_ptr->m_height;
}

const SGRootNode * SGScene::rootNode() const
{
    return &d_ptr->m_root;
}

void SGScene::remove(SGNode * item)
{
    auto it = std::find(d_ptr->m_nodes.begin(), d_ptr->m_nodes.end(), item);
    assert(d_ptr->m_nodes.end() != it);

    d_ptr->m_nodes.erase(it);
    item->d_ptr->m_scene = nullptr;
    itemDeleted(item);
    for (auto child : item->d_ptr->m_children)
        remove(child);
}

void SGScene::add(SGNode * item)
{
    assert(d_ptr->m_nodes.end() == std::find(d_ptr->m_nodes.begin(), d_ptr->m_nodes.end(), item));

    d_ptr->m_nodes.push_back(item);
    item->d_ptr->m_scene = this;
    itemAdded(item);
    for (auto child : item->d_ptr->m_children)
        add(child);
}

void SGScene::addView(SGView* view)
{
    d_ptr->m_views.push_back(view);

    for (auto node : d_ptr->m_nodes)
        view->itemAdded(node);
}

void SGScene::delView(SGView * view)
{
    d_ptr->m_views.remove(view);
}
