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
        UndefinedNodeType
    };
    enum E_DirtyType {
        GeometryDirty = 0x01,
        VisibleDirty = 0x02,
        OpacityDirty = 0x04,
        TreeDirty = 0x08
    };
    //SGNode(SGNode* parent = nullptr);
    virtual ~SGNode();
    E_NodeType type() const;
    void addChild(SGNode* node);
    void removeChild(SGNode* child);
    Rect boundingRect() const;

    int x()const;
    int y()const;
    int z()const;
    int width()const;
    int height()const;
    float opactity()const;
    float rotation()const;
    bool visible()const;
    void setX(int x);
    void setY(int y);
    void setZ(int z);
    void setWidth(int width);
    void setHeight(int height);
    void setOpactity(float opacity);
    void setRotation(float rotation);
    void setVisible(bool visible = true);

    //advance interface
    void setGeometry(int x, int y, int wid, int hei, float rot);

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
