#include "sgrectanglenode.h"

#include "geometry/sgrectanglegeometry.h"
#include "material/sgcolormaterial.h"
#include "material/sgtexturematerial.h"
#include "material/sgyuvmaterial.h"

SGRectangleNode::SGRectangleNode(SGNode * parent):
    SGNode(E_NodeType::RectangleNodeType, parent)
{
    m_material = new SGFlatColorMaterial;
    m_geometry = m_colorGeometry = new SGColorRectangleGeometry;
}

void SGRectangleNode::setColor(Color c)
{
    m_colorGeometry->setColor(c);
}

SGImageNode::SGImageNode(SGNode * parent) :
    SGNode(E_NodeType::ImageNodeType, parent)
{
    m_material = m_texMaterial = new SGTextureMaterial;
    m_geometry = m_texGeometry = new SGTextureRectangleGeometry;
}

void SGImageNode::setImage(const char* src)
{
    const RectF &rec = m_texMaterial->setImage(src);
    
    m_texGeometry->setTexCoord(rec);
}

SGYuvVideoNode::SGYuvVideoNode(SGNode * parent):
    SGNode(E_NodeType::YuvNodeType, parent)
{
    //set default size
    setSize();
    m_material = m_texMaterial = new SGYuvMaterial;
    m_geometry = m_texGeometry = new SGTextureRectangleGeometry;
}

void SGYuvVideoNode::setSize(int pxWid, int pxHei)
{
    assert(pxWid > 0 && pxHei > 0);
    if (m_yuv) free(m_yuv);
    m_pxLen = pxWid*pxHei * 3;
    m_pxLen >>= 1;
    m_yuv = (char*)malloc(m_pxLen);

    m_pxSize.width = pxWid;
    m_pxSize.height = pxHei;
}

bool SGYuvVideoNode::setYuv(const unsigned char * yuv, int len)
{
    if (!yuv) return false;
    if (len >= 0 && len < m_pxLen) return false;

    m_texMaterial->setYuv(yuv);
    return true;
}
