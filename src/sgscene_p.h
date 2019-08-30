#ifndef SGSCENE_P_H
#define SGSCENE_P_H

#include "sgrootnode_p.h"
#include "sgglobal.h"

class SGView;
class SGScenePrivate
{
public:
    std::list<SGView*> m_views;//view obeserver

    SGRootNode m_root;

    //use list, it can store parent and children from first to end.
    std::list<SGNode*> m_nodes;

    float m_width = 1920;
    float m_height = 1080;

    DECLARE_P(SGScene);
};

#endif // SGSCENE_P_H
