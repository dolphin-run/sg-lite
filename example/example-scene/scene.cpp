#include "scene.h"

//test
#include "node/sgrectanglenode.h"
#include "node/sgcircularnode.h"
#include "node/sgtextnode.h"

#include "later.h"

Scene::Scene()
{
    testAlloc();
    Later calllater(1000, true, 1, [this] {this->testUpdate(); });

}

void Scene::testAlloc()
{
#if 1

    for (int i = 0; i < 20; i++)
    {
        //add rectangle
        int row = i / 10;
        int col = i % 10;
        SGRectangleNode *sn = new SGRectangleNode();
        sn->setX(col * 200);
        sn->setY(row * 200);
        sn->setWidth(150);
        sn->setHeight(100);

        sn->setColor(Color::random());

        this->addItem(sn);
        m_nodes.push_back(sn);

        //add image
        SGImageNode *sn1 = new SGImageNode(sn);
        sn1->setX(25);
        sn1->setWidth(100);
        sn1->setHeight(100);

        sn1->setImage("assets/images/img0.png");

        m_nodes.push_back(sn1);
    }

    //add some ring
    for (int i = 0; i < 10; i++)
    {
        int row = i / 10;
        int col = i % 10;
        SGCircularNode *sn = new SGCircularNode();
        sn->setX(col * 200);
        sn->setY(500 + row * 200);
        sn->setZ(3);
        sn->setWidth(150);
        sn->setHeight(150);

        sn->setColor(Color::random(), Color::random());

        this->addItem(sn);
        m_nodes.push_back(sn);
        //break;
    }
#endif // 0
    
    {
        SGTextNode *sn = new SGTextNode();
        this->addItem(sn);
        sn->setX(500);
        sn->setY(760);
        sn->setZ(3);
        sn->setText(L"ABCDEFGHIj\n1234567890");
        //sn->setWidth(100);//auto width

        sn = new SGTextNode();
        this->addItem(sn);
        sn->setX(500);
        sn->setY(700);
        sn->setZ(1);
        sn->setText(L"ABCDEFGHIj1234567890");

        SGImageNode *sn1 = new SGImageNode();
        this->addItem(sn1);
        sn1->setX(600);
        sn1->setY(700);
        sn1->setZ(2);
        sn1->setWidth(100);
        sn1->setHeight(100);
        sn1->setImage("assets/images/img0.png");
    }
}

void Scene::testUpdate()
{
    int ts = 1000;
    while (ts--)
    {
        for (auto it : m_nodes)
        {
            it->setWidth(100 + int(it->width() + 1) % 100);
            it->setHeight(100 + int(it->width() + 1) % 100);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }
}
