#include "sgnode_p.h"
#include "sgrootnode_p.h"
#include "sgscene.h"
#include "sggeometry.h"
#include "sgmaterial.h"

#include <algorithm>


//SGNode::SGNode(SGNode* parent)
//{
//    if (parent)
//    {
//        parent->addChild(this);
//    }
//}
//SGNode::SGNode(E_NodeType type, SGNode * parent)
//{
//    SGNode(type, new SGNodePrivate, parent);
//}

SGNode::SGNode(E_NodeType type, SGNodePrivate* p, SGNode * parent)
{
    SG_INIT_PRIVATE(p);

    d_ptr->m_type = type;

    if (parent)
    {
        if(parent->scene())
        {
            //parent has in a scene.
            parent->scene()->addItem(this, parent);
        }
        else
        {
            //parent is  an isolated node
            parent->addChild(this);
        }
    }
}

SGScene * SGNode::scene()
{
    return d_ptr->m_scene;
}

void SGNode::markDirty(E_DirtyType dirty)
{
    if (d_ptr->m_scene)
        d_ptr->m_scene->itemChanged(this, dirty);
}

bool SGNode::syncState()
{
    return false;
}

bool SGNode::compare(const SGNode *node) const
{
    return true;
}

SGNode::~SGNode()
{
    if (d_ptr->m_geometry) delete d_ptr->m_geometry;
    if (d_ptr->m_material) delete d_ptr->m_material;

    SG_FREE_PRIVATE();
}

SGNode::E_NodeType SGNode::type() const
{
    return d_ptr->m_type;
}

void SGNode::addChild(SGNode* node)
{
    node->d_ptr->m_parent = this;
    //m_children.push_back(node);
    //sort when insert
    auto it = std::upper_bound(d_ptr->m_children.begin(), d_ptr->m_children.end(), node, [](const SGNode *lop, const SGNode *rop) {
        return lop->d_ptr->m_z < rop->d_ptr->m_z;
    });
    d_ptr->m_children.insert(it, node);
}

void SGNode::removeChild(SGNode * child)
{
    assert(child->d_ptr->m_parent);
    assert(d_ptr->m_children.end() != std::find(d_ptr->m_children.begin(), d_ptr->m_children.end(), child));

    d_ptr->m_children.remove(child);
    child->d_ptr->m_parent = nullptr;
}

Rect SGNode::boundingRect() const
{
    Rect rc;
    rc.set(d_ptr->m_x, d_ptr->m_y + d_ptr->m_height, d_ptr->m_x + d_ptr->m_width, d_ptr->m_y);
    return rc;
}

float SGNode::x() const
{
    return d_ptr->m_x;
}

float SGNode::y() const
{
    return d_ptr->m_y;
}

int SGNode::z() const
{
    return d_ptr->m_z;
}

float SGNode::width() const
{
    return d_ptr->m_width;
}

float SGNode::height() const
{
    return d_ptr->m_height;
}

float SGNode::implicitWidth() const
{
    return d_ptr->m_implicitWidth;
}

float SGNode::implicitHeight() const
{
    return d_ptr->m_implicitHeight;
}

float SGNode::opactity() const
{
    return d_ptr->m_opacity;
}

float SGNode::rotation() const
{
    return d_ptr->m_rotation;
}

bool SGNode::visible() const
{
    return d_ptr->m_visible;
}

void SGNode::setX(float x)
{
    d_ptr->m_x = x;
    d_ptr->m_implicitX = x;
    markDirty(SGNode::E_DirtyType::PositionDirty);
}

void SGNode::setY(float y)
{
    d_ptr->m_y = y;
    d_ptr->m_implicitY = y;
    markDirty(SGNode::E_DirtyType::PositionDirty);
}

void SGNode::setZ(int z)
{
    d_ptr->m_z = z;
    markDirty(SGNode::E_DirtyType::OrderingDirty);
    if (d_ptr->m_parent)
    {
        //a more better solution can be remove and insert again.
        d_ptr->m_parent->d_ptr->m_children.sort([](const SGNode *lop, const SGNode *rop) {
            return lop->d_ptr->m_z < rop->d_ptr->m_z;
        });
    }
}

void SGNode::setWidth(float width)
{
    d_ptr->m_width = width;
    d_ptr->m_implicitWidth = width;
    markDirty(SGNode::E_DirtyType::SizeDirty);
}

void SGNode::setHeight(float height)
{
    d_ptr->m_height = height;
    d_ptr->m_implicitHeight = height;
    markDirty(SGNode::E_DirtyType::SizeDirty);
}

void SGNode::setOpactity(float opacity)
{
    d_ptr->m_opacity = opacity;
    markDirty(SGNode::E_DirtyType::OpacityDirty);
}

void SGNode::setRotation(float rotation)
{
    d_ptr->m_rotation = rotation;
    markDirty(SGNode::E_DirtyType::TransformDirty);
}

void SGNode::setScale(float scale)
{
    d_ptr->m_scale = scale;
    markDirty(SGNode::E_DirtyType::TransformDirty);
}

void SGNode::setVisible(bool visible)
{
    d_ptr->m_visible = visible;
    markDirty(SGNode::E_DirtyType::VisibilityDirty);
}

void SGNode::setPos(float x, float y)
{
    d_ptr->m_x = x;
    d_ptr->m_y = y;
    d_ptr->m_implicitX = x;
    d_ptr->m_implicitY = y;

    markDirty(SGNode::E_DirtyType::PositionDirty);
}

void SGNode::setSize(float wid, float hei)
{
    d_ptr->m_width = wid;
    d_ptr->m_implicitWidth = wid;
    d_ptr->m_height = hei;
    d_ptr->m_implicitHeight = hei;

    markDirty(SGNode::E_DirtyType::SizeDirty);
}

SGNodePrivate::~SGNodePrivate() {}

SGRootNode::SGRootNode() : SGNode(SGNode::E_NodeType::RootNodeType, new SGNodePrivate)
{

}