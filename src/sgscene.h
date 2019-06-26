#ifndef SGSCENE_H
#define SGSCENE_H

#include "sgnode.h"
class QSGNode;
class SGScene
{
    friend class SGView;
public:
    SGScene();
    void build();
    void walk();

protected:
    void testAlloc();
    SGRootNode m_root;

    int m_width = 1920;
    int m_height = 1080;

    //QMatrix4x4 m_projection;
};

#endif // SGSCENE_H
