#include "sgcircularnode.h"
#include "geometry/sgcirculargeometry.h"
#include "material/sgcolormaterial.h"

SGCircularNode::SGCircularNode(SGNode * parent):
    SGNode(E_NodeType::CircularNodeType, parent)
{
    m_material = new SGFlatColorMaterial;
    m_geometry = m_colorGeometry = new SGCircularGeometry;
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
