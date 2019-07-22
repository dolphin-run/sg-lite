#include "scenevideo.h"

//test
#include "src/node/sgrectanglenode.h"
#include "src/node/sgcircularnode.h"
#include "src/node/sgtextnode.h"
#include <QTimerEvent>
#include <mutex>

static int timer1 = 0;
static int timer2 = 0;

static const int yuvsize = 1920 * 540 * 3;
static unsigned char *pyuv;
std::mutex plock;
SceneVideo::SceneVideo()
{
    testAlloc();
    timer1 = startTimer(50);
    timer2 = startTimer(5000);

    m_device = SingletonDeviceStream::instance()->spawn();
    m_device->setReceiver(this);

    pyuv = (unsigned char *)malloc(yuvsize);
    memset(pyuv, 0, yuvsize);
    m_device->start();
}

void SceneVideo::recvVideoBlob(const PureVideoBlob * video, const SmartDataBlob * data)
{
    std::lock_guard<std::mutex> guard(plock);
    memcpy(pyuv, video->pixels, video->size);
    //bool ok = m_frames->produce(video->pixels, video->size, data->data, data->size);
}

void SceneVideo::recvCommnBlob(const SmartDataBlob * data)
{
}

void SceneVideo::timerEvent(QTimerEvent * event)
{
    auto id = event->timerId();
    if (timer1 == id)
    {
        std::lock_guard<std::mutex> guard(plock);
        m_videoNode->setYuv(pyuv);
    }
    else if (timer2 == id)
    {
        if (m_videoNode1)
        {
            std::lock_guard<std::mutex> guard(plock);
            m_videoNode1->setYuv(pyuv);
        }
        
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

void SceneVideo::testAlloc()
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
        sn->setInnerRadius(i / 10.f);

        sn->setColor(Color::random(), Color::random());

        this->addItem(sn);
        m_nodes.append(sn);
        //break;
    }
#endif // 0
    
    {
        SGTextNode *sn = new SGTextNode();
        this->addItem(sn);
        sn->setX(800);
        sn->setY(760);
        sn->setZ(3);
        sn->setText(L"iQgdKjABCDEFGH司马懿一心29。ik,.\"'\nhello");
        //sn->setWidth(100);//auto width

        sn = new SGTextNode();
        this->addItem(sn);
        sn->setX(700);
        sn->setY(700);
        sn->setZ(1);
        sn->setText(L"idaeswhjfrewfew");

        SGImageNode *sn1 = new SGImageNode();
        this->addItem(sn1);
        sn1->setX(760);
        sn1->setY(700);
        sn1->setZ(2);
        sn1->setWidth(100);
        sn1->setHeight(100);
        sn1->setImage("images/img0.png");

        m_videoNode = new SGYuvVideoNode();
        this->addItem(m_videoNode);
        m_videoNode->setX(100);
        m_videoNode->setY(700);
        m_videoNode->setWidth(600);
        m_videoNode->setHeight(400);

        m_videoNode1 = new SGYuvVideoNode();
        this->addItem(m_videoNode1);
        m_videoNode1->setX(720);
        m_videoNode1->setY(700);
        m_videoNode1->setWidth(600);
        m_videoNode1->setHeight(400);
    }
}
