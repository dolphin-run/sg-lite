#ifndef SCENE_H
#define SCENE_H

#include "sgscene.h"
#include <list>

class Scene : public SGScene
{
    friend class SGView;
public:
    Scene();

private:
    void testAlloc();
    void testUpdate();
    std::list<SGNode*> m_nodes;
};

#endif // SCENE_H
