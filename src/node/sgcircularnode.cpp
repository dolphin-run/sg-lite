#include "sgcircularnode.h"
#include "geometry/sgcirculargeometry.h"
#include "material/sgcolormaterial.h"
#include "sgnode_p.h"

SGCircularNode::SGCircularNode(SGNode * parent):
    SGNode(E_NodeType::CircularNodeType, parent)
{
    m_colorGeometry = new SGCircularGeometry;
    setGeometry(m_colorGeometry);
    setMaterial(new SGFlatColorMaterial);
}

void SGCircularNode::setInnerRadius(float rate)
{
    m_colorGeometry->setInnerRadius(rate);
}

void SGCircularNode::setColor(const Color &c)
{
    m_colorInside = m_colorOutside = c;

    m_colorGeometry->setColor(m_colorInside);
}

void SGCircularNode::setColor(const Color &inside, const Color &outside)
{
    m_colorInside = inside;
    m_colorOutside = outside;

    m_colorGeometry->setColor(m_colorInside, m_colorOutside);
}
