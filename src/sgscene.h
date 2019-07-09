#ifndef SGSCENE_H
#define SGSCENE_H

#include "sgnode.h"
class QSGNode;
class SGScene
{
    friend class SGView;
public:
    SGScene();

    void addItem(SGNode *item);

private:
    SGRootNode m_root;

    int m_width = 1920;
    int m_height = 1080;
};

#endif // SGSCENE_H
