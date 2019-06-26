#include "sgrectanglenode.h"

#include "geometry/sgrectanglegeometry.h"
#include "material/sgcolormaterial.h"
#include "material/sgtexturematerial.h"

SGRectangleNode::SGRectangleNode(SGNode * parent):
    SGNode(parent)
{
    m_material = new SGFlatColorMaterial;
    m_geometry = m_colorGeometry = new SGColorRectangleGeometry;
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

    m_colorGeometry->setColor(m_color);
}

SGImageNode::SGImageNode(SGNode * parent)
{
    m_material = new SGTextureMaterial;
    m_geometry = m_texGeometry = new SGTextureRectangleGeometry;
}

void SGImageNode::setImage(int32_t c)
{
    //10,5
    int row = c / 10;
    int col = c % 10;
    m_texGeometry->setTexCoord(0.2f*(row + 1), 0.1f*col, 0.1f*(col + 1), 0.2f*row);
}
