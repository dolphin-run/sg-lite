#ifndef SGNODE_H
#define SGNODE_H
#include <list>
#include <assert.h>

#include "sgstruct.h"
#include "sggeometry.h"
#include "sgmaterial.h"


class SGNode
{
public:
    enum E_NodeType {
        RootNodeType, 
        RectangleNodeType,
        CircularNodeType,
        ImageNodeType,
        UndefinedNodeType
    };
    enum E_DirtyType {
        GeometryDirty = 0x01,
        OpacityDirty = 0x02
    };
    SGNode(SGNode* parent = nullptr);
    virtual ~SGNode();
    E_NodeType type() const;
    void add(SGNode* node);
    Rect boundingRect() const;

    int x()const;
    int y()const;
    int z()const;
    int width()const;
    int height()const;
    float opactity()const;
    bool visible()const;
    void setX(int x);
    void setY(int y);
    void setZ(int z);
    void setWidth(int width);
    void setHeight(int height);
    void setOpactity(float opacity);
    void setVisible(bool visible = true);

protected:
    SGNode(E_NodeType type, SGNode* parent = nullptr);
    //properties
    int m_x = 0;//left
    int m_y = 0;//top
    int m_z = 0;//z-index
    int m_width = 0;//in scene's pixel size
    int m_height = 0;

    float m_opacity = 1.f;
    float m_scale = 1.f;
    bool m_visible = true;

    SGNode* m_parent = nullptr;
    E_NodeType m_type = RootNodeType;
    std::list<SGNode*> m_children;

    SGGeometry *m_geometry = nullptr;
    SGMaterial *m_material = nullptr;
    mutable unsigned m_dirty = 0;

    friend class Node;
    friend class SGRenderer;
    friend class SGScene;
};

class SGRootNode : public SGNode
{
public:
    SGRootNode();
};

#endif // SGNODE_H
