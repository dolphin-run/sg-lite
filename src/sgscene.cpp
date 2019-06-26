#include "sgscene.h"

SGScene::SGScene()
{

}

void SGScene::build()
{
    m_projection.setToIdentity();
    m_projection.ortho(0, m_width, m_height, 0, -1, 1);
}

void SGScene::walk()
{
    //m_opacity_nodes.append();
}

void SGScene::test()
{
    SGScene scene;
    scene.build();
}
