#ifndef SGSCENE_H
#define SGSCENE_H

#include "sgnode.h"
class QSGNode;
class SGScene
{
    friend class SGView;
public:
    SGScene();

    //to do...classify in the future.
    void boundingRectChanged();
    void itemAdded();
    void itemChanged();

    void addItem(SGNode *item);

protected:
    std::list<SGView*> m_views;//view obeserver

private:
    SGRootNode m_root;

    int m_width = 1920;
    int m_height = 1080;
};

#endif // SGSCENE_H
