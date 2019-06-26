#include "sgcircularnode.h"
#include "../geometry/sgcirculargeometry.h"
#include "../material/sgcolormaterial.h"

SGCircularNode::SGCircularNode(SGNode * parent)
{
    m_material = new SGFlatColorMaterial;
    m_geometry = m_colorGeometry = new SGCircularGeometry;
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

    m_colorGeometry->setColor(m_color);
}
