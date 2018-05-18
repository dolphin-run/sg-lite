#ifndef SCENE_H
#define SCENE_H

#include "sgscene.h"
#include <list>
#include "later.h"

class Scene : public SGScene
{
    friend class SGView;
public:
    Scene();
    ~Scene();

private:
    void testAlloc();
    void testUpdate();
    std::list<SGNode*> m_nodes;

private:
    Later *m_timer;
};

#endif // SCENE_H
