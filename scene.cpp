#include "scene.h"

//test
#include "src/node/sgrectanglenode.h"
#include "src/node/sgcircularnode.h"
#include "src/node/sgtextnode.h"
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
            if (it->type() != SGNode::E_NodeType::TextNodeType)
            {
                it->setWidth(100 + (it->width() + 1) % 100);
                it->setHeight(100 + (it->width() + 1) % 100);
            }
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
                    imgNode->setImage("images/img1.png");
                else
                    imgNode->setImage("images/img2.png");
            }
        }
    }
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
        m_nodes.append(sn);

        //add image
        SGImageNode *sn1 = new SGImageNode(sn);
        sn1->setX(25);
        sn1->setWidth(100);
        sn1->setHeight(100);

        sn1->setImage("images/img0.png");

        m_nodes.append(sn1);
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
        m_nodes.append(sn);
        //break;
    }
#endif // 0
    
    {
        SGTextNode *sn = new SGTextNode();
        this->addItem(sn);
        sn->setX(500);
        sn->setY(760);
        sn->setZ(3);
        sn->setText(L"iQgdKjABCDEFGH司马懿一心29。ik,.\"'\nhello");
        //sn->setWidth(100);//auto width

        sn = new SGTextNode();
        this->addItem(sn);
        sn->setX(500);
        sn->setY(700);
        sn->setZ(1);
        sn->setText(L"idaeswhjfrewfew");

        SGImageNode *sn1 = new SGImageNode();
        this->addItem(sn1);
        sn1->setX(600);
        sn1->setY(700);
        sn1->setZ(2);
        sn1->setWidth(100);
        sn1->setHeight(100);
        sn1->setImage("images/img0.png");
    }
}
