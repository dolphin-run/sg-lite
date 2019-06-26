#include "sgnode.h"




SGNode::SGNode(SGNode* parent)
{
    if(parent) parent->add(this);
}

SGNode::~SGNode()
{
    if (m_geometry) delete m_geometry;
    if (m_material) delete m_material;
}

void SGNode::add(SGNode* node)
{
    node->m_parent = this;
    m_children.push_back(node);
}

Rect SGNode::boundingRect() const
{
    Rect rc;
    rc.set(x, y+height, x+width, y);
    return rc;
}

void SGNode::renderProcessor()
{
    //rx = 
}

void SGNode::updateMatrix()
{
    matrix.setToIdentity();
    matrix.translate(x + width / 2, y + height / 2);
    matrix.scale(width, height);
}
