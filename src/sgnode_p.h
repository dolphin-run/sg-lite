#ifndef SGNODE_P_H
#define SGNODE_P_H
#include "sgnode.h"

#include <list>

class SGScene;
class SGGeometry;
class SGMaterial;
class SGNode;
class SGNodePrivate
{
    SG_DECLARE_PUBLIC(SGNode);
public:
    virtual ~SGNodePrivate();

    //properties
    float m_x = 0;//left
    float m_y = 0;//top
    int m_z = 0;//z-index
    float m_width = 0;//in scene's pixel size
    float m_height = 0;
    float m_implicitX = 0;
    float m_implicitY = 0;
    float m_implicitWidth = 0;
    float m_implicitHeight = 0;

    float m_opacity = 1.f;
    float m_scale = 1.f;
    float m_rotation = 0.f;
    bool m_visible = true;

    SGNode* m_parent = nullptr;
    SGScene *m_scene = nullptr;//should only be modified in the SGScene
    SGNode::E_NodeType m_type = SGNode::RootNodeType;
    std::list<SGNode*> m_children;

    SGGeometry *m_geometry = nullptr;
    SGMaterial *m_material = nullptr;

    SGNode *q_ptr;
};

#endif // SGNODE_P_H
