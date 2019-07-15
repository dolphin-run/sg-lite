#include "sgscene.h"
#include "sgview.h"

SGScene::SGScene()
{
}

void SGScene::sizeChanged()
{
    for (auto view : m_views)
    {
        view->sceneChanged();
    }
}

void SGScene::itemAdded(SGNode *item)
{
    for (auto view : m_views)
    {
        view->itemAdded(item);
    }
}

void SGScene::itemDeleted(SGNode *item)
{
    for (auto view : m_views)
    {
        view->itemDeleted(item);
    }
}

void SGScene::itemChanged(SGNode *item)
{
    for (auto view : m_views)
    {
        view->itemChanged(item);
    }
}

bool SGScene::addItem(SGNode * item, SGNode *attached)
{
    if (m_nodes.count(item)) return false;

    if (!attached)
    {
        m_root.addChild(item);
    }
    else
    {
        assert(attached->m_scene == this);
        attached->addChild(item);
    }
    add(item);

    return true;
}

bool SGScene::removeItem(SGNode * item)
{
    if (m_nodes.count(item))
    {
        if (item->m_parent)
        {
            item->m_parent->remove(item);
        }
        remove(item);

        return true;
    }
    return false;
    
}

void SGScene::clear()
{
    for (auto itm : m_nodes)
        delete itm;
    m_nodes.clear();
}

void SGScene::remove(SGNode * item)
{
    assert(m_nodes.count(item));

    m_nodes.erase(item);
    item->m_scene = nullptr;
    itemDeleted(item);
    for (auto child : item->m_children)
        remove(child);
}

void SGScene::add(SGNode * item)
{
    assert(!m_nodes.count(item));

    m_nodes.insert(item);
    item->m_scene = this;
    itemAdded(item);
    for (auto child : item->m_children)
        add(child);
}

void SGScene::addView(SGView* view)
{
    m_views.push_back(view);

    for (auto node : m_nodes)
        view->itemAdded(node);
}
