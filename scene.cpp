#include "scene.h"

//test
#include "src/node/sgrectanglenode.h"
#include "src/node/sgcircularnode.h"
#include <QTimerEvent>

int timer1 = 0;
int timer2 = 0;

Scene::Scene()
{
    testAlloc();
    timer1 = startTimer(20);
    timer2 = startTimer(5000);
}

void Scene::timerEvent(QTimerEvent * event)
{
    auto id = event->timerId();
    if (timer1 == id)
    {
        for (auto it : m_nodes)
        {
            it->setWidth(100 + (it->width() + 1) % 100);
            it->setHeight(100 + (it->width() + 1) % 100);
        }
    }
    else if (timer2 == id)
    {
        static bool odd = true;
        odd = !odd;
        for (auto it : m_nodes)
        {
            if (it->type() == SGNode::E_NodeType::ImageNodeType)
            {
                SGImageNode* imgNode = dynamic_cast<SGImageNode*>(it);
                if(odd)
                    imgNode->setImage("F:/doc/mk/VAT/entity/en/256/1-Person/Courier.png");
                else
                    imgNode->setImage("F:/doc/mk/VAT/entity/en/256/1-Person/Man.png");
            }
        }
    }
}

void Scene::testAlloc()
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

        sn->setColor(Color::random());

        this->addItem(sn);
        m_nodes.append(sn);
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

        sn->setColor(Color::random(), Color::random());

        this->addItem(sn);
        m_nodes.append(sn);
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

        sn->setImage("F:/doc/mk/VAT/entity/en/256/1-Person/Man.png");

        this->addItem(sn);
        m_nodes.append(sn);
    }
}
