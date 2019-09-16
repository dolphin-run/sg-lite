#ifndef SCENEVIDEO_H
#define SCENEVIDEO_H

#include "sgscene.h"
#include <thread>
#include <atomic>
#include <list>

class SGYuvVideoNode;
class SceneVideo : public SGScene
{
public:
    SceneVideo();
    ~SceneVideo();

    void start();
    void stop();

    void syncYuv();
    void taskYuvStream();

protected:
    std::thread m_reader;
    std::atomic_bool m_runningflag;
    std::atomic_bool m_producingflag;

protected:
    void testAlloc();

    std::list<SGNode *> m_nodes;
    SGYuvVideoNode *m_videoNode = nullptr;
    SGYuvVideoNode *m_videoNode1 = nullptr;
};

#endif // SCENEVIDEO_H
