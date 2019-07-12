#include "sgscene.h"
#include "sgview.h"

SGScene::SGScene()
{
}

void SGScene::boundingRectChanged()
{
    for (auto view : m_views)
    {
        view->sceneChanged();
    }
}

void SGScene::itemAdded()
{
}

void SGScene::itemChanged()
{
}

void SGScene::addItem(SGNode * item)
{
    m_root.add(item);
}