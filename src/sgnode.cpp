#include "sgnode.h"
#include "sgscene.h"


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
    m_children.push_back(node);
}

void SGNode::remove(SGNode * child)
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

int SGNode::x() const
{
    return m_x;
}

int SGNode::y() const
{
    return m_y;
}

int SGNode::z() const
{
    return m_z;
}

int SGNode::width() const
{
    return m_width;
}

int SGNode::height() const
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

void SGNode::setX(int x)
{
    m_x = x;
    m_dirty |= E_DirtyType::GeometryDirty;
}

void SGNode::setY(int y)
{
    m_y = y;
    m_dirty |= E_DirtyType::GeometryDirty;
}

void SGNode::setZ(int z)
{
    m_z = z;
    m_dirty |= E_DirtyType::GeometryDirty;
}

void SGNode::setWidth(int width)
{
    m_width = width;
    m_dirty |= E_DirtyType::GeometryDirty;
}

void SGNode::setHeight(int height)
{
    m_height = height;
    m_dirty |= E_DirtyType::GeometryDirty;
}

void SGNode::setOpactity(float opacity)
{
    m_opacity = opacity;
}

void SGNode::setRotation(float rotation)
{
    m_rotation = rotation;
}

void SGNode::setVisible(bool visible)
{
    m_visible = visible;
}

SGRootNode::SGRootNode() : SGNode(SGNode::E_NodeType::RootNodeType)
{

}