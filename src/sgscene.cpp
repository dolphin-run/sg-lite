#include "sgscene.h"

SGScene::SGScene()
{
}

void SGScene::addItem(SGNode * item)
{
    m_root.add(item);
}