#ifndef SCENEFANTACY_H
#define SCENEFANTACY_H

#include "sgscene.h"

#include "later.h"
#include <vector>

class SceneFantacy : public SGScene
{
    friend class SGView;
public:
    SceneFantacy();
    ~SceneFantacy();

protected:
    void timerEvent();
    void testSpawn();
    void testAlloc();
    void testVisible(int gen);
    void testUpdate();

    std::vector<SGNode *> m_nodes;

private:
    Later *m_timer;
};

#endif // SCENEFANTACY_H
