#ifndef SGSCENE_P_H
#define SGSCENE_P_H

#include "sgrootnode_p.h"
#include "sgglobal.h"

#include <list>

class SGView;
class SGScenePrivate
{
    SG_DECLARE_PUBLIC(SGScene);
public:
    std::list<SGView*> m_views;//view obeserver

    SGRootNode m_root;

    //use list, it can store parent and children from first to end.
    std::list<SGNode*> m_nodes;

    float m_width = 1920;
    float m_height = 1080;

    SGScene *q_ptr;
};

#endif // SGSCENE_P_H
