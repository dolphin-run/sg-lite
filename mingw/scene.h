#ifndef SCENE_H
#define SCENE_H

#include "sgscene.h"

class Scene : public SGScene
{
    friend class SGView;
public:
    Scene();

protected:
    void testAlloc();
};

#endif // SCENE_H
