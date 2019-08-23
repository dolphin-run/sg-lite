#include "scenevideo.h"

//test
#include "src/node/sgrectanglenode.h"
#include "src/node/sgcircularnode.h"
#include "src/node/sgtextnode.h"
#include <iostream>
#include <assert.h>

static int timer = 0;

static const int yuvsize = 1920 * 540 * 3;
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
    FILE *file = fopen("assets/video/data.dat", "rb");
    if (!file)
    {
        std::cout << "open file failed..." << std::endl;
        return;
    }

    int dattype = 7;
    int datalen = 0;
    while (m_producingflag == true)
    {
        int cnt = fread(&dattype, 1, 4, file);
        if (cnt != 4)
        {
            fseek(file, 0, SEEK_SET);
            continue;
        }

        printf("[%d]\n", dattype);

        //smart video
        if (dattype == 0)
        {
            cnt = fread(&datalen, 1, 4, file);
            assert(datalen == yuvsize);
            cnt = fread(yuvbuf, 1, datalen, file);
            syncYuv();
        }

        fread(&datalen, 1, 4, file);

        if (datalen>0)
        {
            fseek(file, datalen, SEEK_CUR);
            //cnt = fread(, 1, datalen, file);
        }

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
    this->addItem(m_videoNode);
    m_videoNode->setX(0);
    m_videoNode->setY(0);
    m_videoNode->setWidth(960);
    m_videoNode->setHeight(540);

    m_videoNode1 = new SGYuvVideoNode();
    this->addItem(m_videoNode1);
    m_videoNode1->setX(960);
    m_videoNode1->setY(0);
    m_videoNode1->setWidth(960);
    m_videoNode1->setHeight(540);
}
