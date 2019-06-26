#include "sgscene.h"

SGScene::SGScene()
{
}

void SGScene::build()
{
    //m_projection.setToIdentity();
    //m_projection.ortho(0, m_width, m_height, 0, -1, 1);
    testAlloc();
}

void SGScene::walk()
{
    //m_opacity_nodes.append();
}


#include "node/sgrectanglenode.h"
#include "node/sgcircularnode.h"
#if 0
void SGScene::testAlloc()
{
    //add some rectangle
    SGRectangleNode *sn0 = new SGRectangleNode(&m_root);
    sn0->setX(100);
    sn0->setY(50);
    sn0->setWidth(400);
    sn0->setHeight(100);
    sn0->setColor(0xff0000);
    SGRectangleNode *sn1 = new SGRectangleNode(&m_root);
    sn1->setX(800);
    sn1->setY(200);
    sn1->setWidth(400);
    sn1->setHeight(100);
    sn1->setColor(0x00ff00);
    SGRectangleNode *sn2 = new SGRectangleNode(&m_root);
    sn2->setX(300);
    sn2->setY(250);
    sn2->setWidth(400);
    sn2->setHeight(100);
    sn2->setColor(0x0000ff);
    SGRectangleNode *sn3 = new SGRectangleNode(&m_root);
    sn3->setX(1300);
    sn3->setY(750);
    sn3->setWidth(400);
    sn3->setHeight(100);

    //add some ring
    SGCircularNode *sn10 = new SGCircularNode(&m_root);
    sn10->setX(100);
    sn10->setY(550);
    sn10->setWidth(400);
    sn10->setHeight(400);
    sn10->setColor(0x1C86EE);
    SGCircularNode *sn11 = new SGCircularNode(&m_root);
    sn11->setX(0);
    sn11->setY(0);
    sn11->setWidth(400);
    sn11->setHeight(400);
    sn11->setColor(0xCD5C5C);

    //add some image
    SGImageNode *sn20 = new SGImageNode(&m_root);
    sn20->setX(1500);
    sn20->setY(0);
    sn20->setWidth(160);
    sn20->setHeight(160);
    sn20->setImage(5);
    SGImageNode *sn21 = new SGImageNode(&m_root);
    sn21->setX(1500);
    sn21->setY(600);
    sn21->setWidth(80);
    sn21->setHeight(80);
    sn21->setImage(15);
}
#else
void SGScene::testAlloc()
{
    //add some rectangle
    for (int i = 0; i < 20; i++)
    {
        int row = i / 10;
        int col = i % 10;
        SGRectangleNode *sn = new SGRectangleNode(&m_root);
        sn->setX(col * 200);
        sn->setY(row * 200);
        sn->setWidth(150);
        sn->setHeight(100);

        int r = rand() % 256;
        int g = rand() % 256;
        int b = rand() % 256;
        sn->setColor((r << 16) + (g << 8) + b);
    }

    //add some ring
    for (int i = 0; i < 10; i++)
    {
        int row = i / 10;
        int col = i % 10;
        SGCircularNode *sn = new SGCircularNode(&m_root);
        sn->setX(col * 200);
        sn->setY(500 + row * 200);
        sn->setWidth(150);
        sn->setHeight(150);

        int r = rand() % 256;
        int g = rand() % 256;
        int b = rand() % 256;
        int r1 = rand() % 256;
        int g1 = rand() % 256;
        int b1 = rand() % 256;
        sn->setColor((r << 16) + (g << 8) + b, (r1 << 16) + (g1 << 8) + b1);
    }

    //add some image
    for (int i = 0; i < 20; i++)
    {
        int row = i / 10;
        int col = i % 10;
        SGImageNode *sn = new SGImageNode(&m_root);
        sn->setX(col * 200);
        sn->setY(700 + row * 200);
        sn->setWidth(100);
        sn->setHeight(100);

        sn->setImage(i);
    }
}
#endif
