#include "scenevideo.h"

//test
#include "node/sgrectanglenode.h"
#include "node/sgcircularnode.h"
#include "node/sgtextnode.h"
#include <iostream>
#include <assert.h>

static int timer = 0;

#define VIDEO_WIDTH 352
#define VIDEO_HEIGHT 288

static const int yuvsize = VIDEO_WIDTH*VIDEO_HEIGHT * 3 / 2;
static unsigned char *yuvbuf;
SceneVideo::SceneVideo()
{
    m_runningflag.store(false);
    m_producingflag.store(false);

    yuvbuf = (unsigned char *)malloc(yuvsize);
    memset(yuvbuf, 0, yuvsize);

    testAlloc();

    start();
}

SceneVideo::~SceneVideo()
{
    stop();
}

void SceneVideo::start()
{
    if (!m_runningflag == true)
    {
        m_runningflag.store(true);
        m_producingflag.store(true);
        m_reader = std::thread([this] {
            this->taskYuvStream();
        });
        m_reader.detach();
    }
}

void SceneVideo::stop()
{
    if (m_runningflag == true)
    {
        m_producingflag.store(false);
        //wait for finish...
        std::this_thread::sleep_for(std::chrono::milliseconds(80));
    }
}

void SceneVideo::taskYuvStream()
{
    FILE *file = fopen("assets/video/flower_cif.yuv" , "rb");
    if (!file)
    {
        std::cout << "open file failed..." << std::endl;
        return;
    }

    while (m_producingflag == true)
    {
        int cnt = fread(yuvbuf, 1, yuvsize, file);
        if (cnt != yuvsize)
        {
            fseek(file, 0, SEEK_SET);
            continue;
        }

        syncYuv();

        std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }

    fclose(file);
    printf("thread finished...");
    m_runningflag.store(false);
}

void SceneVideo::syncYuv()
{
    timer++;

    m_videoNode->setYuv(yuvbuf);
    
    if (0 == timer%40)
    {
        if (m_videoNode1)
        {
            m_videoNode1->setYuv(yuvbuf);
        }
    }
}

void SceneVideo::testAlloc()
{
    m_videoNode = new SGYuvVideoNode();
    m_videoNode->setPixelSize(VIDEO_WIDTH, VIDEO_HEIGHT);
    this->addItem(m_videoNode);

    m_videoNode->setX(0);
    m_videoNode->setY(0);
    m_videoNode->setWidth(960);
    m_videoNode->setHeight(540);

    //shadow node
    m_videoNode1 = new SGYuvVideoNode();
    m_videoNode1->setPixelSize(VIDEO_WIDTH, VIDEO_HEIGHT);
    this->addItem(m_videoNode1);

    m_videoNode1->setX(960);
    m_videoNode1->setY(0);
    m_videoNode1->setWidth(960);
    m_videoNode1->setHeight(540);
}
