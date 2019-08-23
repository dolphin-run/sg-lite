#ifndef SGNODE_H
#define SGNODE_H
#include <list>
#include <assert.h>

#include "sgstruct.h"
#include "sggeometry.h"
#include "sgmaterial.h"


class SGScene;
struct Node;
class SGNode
{
public:
    enum E_NodeType {
        RootNodeType, 
        RectangleNodeType,
        CircularNodeType,
        ImageNodeType,
        TextNodeType,
        YuvNodeType,
        DynImageNodeType,
        UndefinedNodeType
    };
    enum E_DirtyType {
        //GeometryDirty
        PositionDirty = 0x01,
        SizeDirty = 0x02,
        TransformDirty = 0x04,//transformation, it will be inherited by its children.
        //
        OrderingDirty = 0x11,
        OpacityDirty = 0x12,
        VisibilityDirty = 0x14,
        //
        AllDirty = 0xff
    };
    //SGNode(SGNode* parent = nullptr);
    virtual ~SGNode();
    E_NodeType type() const;
    void addChild(SGNode* node);
    void removeChild(SGNode* child);
    Rect boundingRect() const;

    float x()const;
    float y()const;
    int z()const;
    float width()const;
    float height()const;
    float opactity()const;
    float rotation()const;
    bool visible()const;
    void setX(float x);
    void setY(float y);
    void setZ(int z);
    void setWidth(float width);
    void setHeight(float height);
    void setOpactity(float opacity);
    void setRotation(float rotation);
    void setVisible(bool visible = true);

    //advance interface
    void setPos(float x, float y);
    void setSize(float wid, float hei);

protected:
    SGNode(E_NodeType type, SGNode* parent = nullptr);

    //something changed to gl is not thread-safe, changes should been made here.
    virtual bool syncState();

    //properties
    float m_x = 0;//left
    float m_y = 0;//top
    int m_z = 0;//z-index
    float m_width = 0;//in scene's pixel size
    float m_height = 0;

    float m_opacity = 1.f;
    float m_scale = 1.f;
    float m_rotation = 0.f;
    bool m_visible = true;

    SGNode* m_parent = nullptr;
    SGScene *m_scene = nullptr;//should only be modified in the SGScene
    E_NodeType m_type = RootNodeType;
    std::list<SGNode*> m_children;

    SGGeometry *m_geometry = nullptr;
    SGMaterial *m_material = nullptr;

    friend struct Node;
    friend class SGRenderer;
    friend class SGScene;
};

class SGRootNode : public SGNode
{
public:
    SGRootNode();
};

#endif // SGNODE_H
