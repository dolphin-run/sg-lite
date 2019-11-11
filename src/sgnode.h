#ifndef SGNODE_H
#define SGNODE_H

#include "sgstruct.h"
#include "sgglobal.h"

class SGScene;
class SGGeometry;
class SGMaterial;
class SGNodePrivate;
class SG_DECL_EXPORT SGNode
{
    SG_DISABLE_COPY(SGNode);
    SG_DECLARE_PRIVATE(SGNode);

    friend class SGRenderer;
    friend class SGScene;
public:
    enum E_NodeType {
        BasicNodeType,
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
    SGNode(SGNode* parent = nullptr);
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
    float implicitWidth()const;
    float implicitHeight()const;
    float opactity()const;
    float rotation()const;
    bool visible()const;
    void setX(float x);
    void setY(float y);
    void setZ(int z);
    void setWidth(float width);
    void setHeight(float height);
    void setOpacity(float opacity);
    void setRotation(float rotation);
    void setScale(float scale);
    void setVisible(bool visible = true);

    //advance interface
    void setPos(float x, float y);
    void setSize(float wid, float hei);

protected:
    SGNode(E_NodeType type, SGNodePrivate* p, SGNode* parent = nullptr);
    SGScene *scene();
    SGNodePrivate *d_ptr;

    void markDirty(E_DirtyType dirty);

    //something changed to gl is not thread-safe, changes should been made here.
    virtual bool syncState();
    virtual bool compare(const SGNode *node) const;
    virtual SGNodeState nodeState() const { return SGNodeState(); }
};

#endif // SGNODE_H
