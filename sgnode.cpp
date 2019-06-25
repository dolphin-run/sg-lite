#include "sgnode.h"
#include <QOpenGLFunctions>




SGNode::SGNode(SGNode* parent)
{
    if(parent) parent->add(this);
}

SGNode::~SGNode()
{
}

void SGNode::add(SGNode* node)
{
    node->m_parent = this;
    m_children.push_back(node);
}

Rect SGNode::boundingRect() const
{
    return Rect();
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

SGRectangleNode::SGRectangleNode(SGNode * parent):
    SGNode(parent)
{
    m_material = new SGFlatColorMaterial;
    m_geometry = &m_colorGeometry;
}

void SGRectangleNode::setColor(int32_t c)
{
//    color.a = c&0xff;
//    c>>=8;
    m_color.b = c&0xff;
    c>>=8;
    m_color.g = c&0xff;
    c>>=8;
    m_color.r = c&0xff;

    m_colorGeometry.setColor(m_color);
}

SGCircularNode::SGCircularNode(SGNode * parent)
{
    m_material = new SGFlatColorMaterial;
    m_geometry = &m_colorGeometry;
}

void SGCircularNode::setColor(int32_t c)
{
    //    color.a = c&0xff;
    //    c>>=8;
    m_color.b = c & 0xff;
    c >>= 8;
    m_color.g = c & 0xff;
    c >>= 8;
    m_color.r = c & 0xff;

    m_colorGeometry.setColor(m_color);
}
