#ifndef SCENEFANTACY_H
#define SCENEFANTACY_H

#include "sgscene.h"
#include <vector>

class SceneFantacy : public SGScene
{
    friend class SGView;
public:
    SceneFantacy();

protected:
    void timerEvent();
    void testSpawn();
    void testAlloc();
    void testVisible(int gen);
    void testUpdate();

    std::vector<SGNode *> m_nodes;
};

#endif // SCENEFANTACY_H
