#include "sgrectanglenode.h"

#include "geometry/sgrectanglegeometry.h"
#include "material/sgcolormaterial.h"
#include "material/sgtexturematerial.h"

SGRectangleNode::SGRectangleNode(SGNode * parent):
    SGNode(E_NodeType::RectangleNodeType, parent)
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

SGImageNode::SGImageNode(SGNode * parent) :
    SGNode(E_NodeType::ImageNodeType, parent)
{
    m_material = m_texMaterial = new SGTextureMaterial;
    m_geometry = m_texGeometry = new SGTextureRectangleGeometry;
}

void SGImageNode::setImage(const char* src)
{
    const RectF &rec = m_texMaterial->addImage(src);
    
    m_texGeometry->setTexCoord(rec);
}
