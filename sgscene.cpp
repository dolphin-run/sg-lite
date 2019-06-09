#include "sgscene.h"

SGScene::SGScene()
{

}

void SGScene::build()
{
    m_root = new SGRectangleNode;
    m_root->x = 100;
    m_root->y = 50;
    m_root->width = 400;
    m_root->height = 100;
    auto n1 = new SGRectangleNode(m_root);
    n1->setColor(0xff0000);
    n1->x = 300;
    n1->y = 150;
    n1->width = 400;
    n1->height = 100;

    auto n2 = new SGRectangleNode(m_root);
    n2->setColor(0x00ff00);
    n2->x = 700;
    n2->y = 250;
    n2->width = 400;
    n2->height = 100;
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
