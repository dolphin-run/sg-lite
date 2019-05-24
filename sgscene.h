#ifndef SGSCENE_H
#define SGSCENE_H

#include "sgnode.h"
class SGScene
{
public:
    SGScene();
    void build();
    void walk();

    static void test();
protected:
    SGNode *m_root = nullptr;

    QList<QSGNode*> m_opacity_nodes;
    QList<QSGNode*> m_alpha_nodes;
};

#endif // SGSCENE_H
