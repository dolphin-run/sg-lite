#include "sgnode.h"
#include "sgscene.h"
#include <algorithm>


//SGNode::SGNode(SGNode* parent)
//{
//    if (parent)
//    {
//        parent->addChild(this);
//    }
//}

SGNode::SGNode(E_NodeType type, SGNode * parent):
    m_type(type)
{
    if (parent)
    {
        if(parent->m_scene)
        {
            //parent has in a scene.
            parent->m_scene->addItem(this, parent);
        }
        else
        {
            //parent is  an isolated node
            parent->addChild(this);
        }
    }
}

SGNode::~SGNode()
{
    if (m_geometry) delete m_geometry;
    if (m_material) delete m_material;
}

SGNode::E_NodeType SGNode::type() const
{
    return m_type;
}

void SGNode::addChild(SGNode* node)
{
    node->m_parent = this;
    //m_children.push_back(node);
    //sort when insert
    auto it = std::upper_bound(m_children.begin(), m_children.end(), node, [](const SGNode *lop, const SGNode *rop) {
        return lop->m_z < rop->m_z;
    });
    m_children.insert(it, node);
}

void SGNode::removeChild(SGNode * child)
{
    assert(child->m_parent);
    assert(m_children.end() != std::find(m_children.begin(), m_children.end(), child));

    m_children.remove(child);
    child->m_parent = nullptr;
}

Rect SGNode::boundingRect() const
{
    Rect rc;
    rc.set(m_x, m_y + m_height, m_x + m_width, m_y);
    return rc;
}

float SGNode::x() const
{
    return m_x;
}

float SGNode::y() const
{
    return m_y;
}

int SGNode::z() const
{
    return m_z;
}

float SGNode::width() const
{
    return m_width;
}

float SGNode::height() const
{
    return m_height;
}

float SGNode::opactity() const
{
    return m_opacity;
}

float SGNode::rotation() const
{
    return m_rotation;
}

bool SGNode::visible() const
{
    return m_visible;
}

void SGNode::setX(float x)
{
    m_x = x;
    if (m_scene)
        m_scene->itemChanged(this, SGNode::E_DirtyType::PositionDirty);
}

void SGNode::setY(float y)
{
    m_y = y;
    if (m_scene)
        m_scene->itemChanged(this, SGNode::E_DirtyType::PositionDirty);
}

void SGNode::setZ(int z)
{
    m_z = z;
    if (m_scene)
        m_scene->itemChanged(this, SGNode::E_DirtyType::OrderingDirty);
    if (m_parent)
    {
        //a more better solution can be remove and insert again.
        m_parent->m_children.sort([](const SGNode *lop, const SGNode *rop) {
            return lop->m_z < rop->m_z;
        });
    }
}

void SGNode::setWidth(float width)
{
    m_width = width;
    if (m_scene)
        m_scene->itemChanged(this, SGNode::E_DirtyType::SizeDirty);
}

void SGNode::setHeight(float height)
{
    m_height = height;
    if (m_scene)
        m_scene->itemChanged(this, SGNode::E_DirtyType::SizeDirty);
}

void SGNode::setOpactity(float opacity)
{
    m_opacity = opacity;
    if (m_scene)
        m_scene->itemChanged(this, SGNode::E_DirtyType::OpacityDirty);
}

void SGNode::setRotation(float rotation)
{
    m_rotation = rotation;
    if (m_scene)
        m_scene->itemChanged(this, SGNode::E_DirtyType::TransformDirty);
}

void SGNode::setVisible(bool visible)
{
    m_visible = visible;
    if (m_scene)
        m_scene->itemChanged(this, SGNode::E_DirtyType::VisibilityDirty);
}

void SGNode::setPos(float x, float y)
{
    m_x = x;
    m_y = y;
    
    if (m_scene)
        m_scene->itemChanged(this, SGNode::E_DirtyType::PositionDirty);
}

void SGNode::setSize(float wid, float hei)
{
    m_width = wid;
    m_height = hei;

    if (m_scene)
        m_scene->itemChanged(this, SGNode::E_DirtyType::SizeDirty);
}

SGRootNode::SGRootNode() : SGNode(SGNode::E_NodeType::RootNodeType)
{

}