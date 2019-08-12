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
    SGNode(E_NodeType::YuvNodeType, parent),
    m_yuvInf(new SYUVInfo)
{
    m_material = m_texMaterial = new SGYuvMaterial;
    m_geometry = m_texGeometry = new SGTextureRectangleGeometry;

    //set default size
    setPixelSize();
}

void SGYuvVideoNode::setPixelSize(int pxWid, int pxHei)
{
    assert(pxWid > 0 && pxHei > 0);

    m_yuvInf->update(pxWid, pxHei);
    m_texMaterial->setYuvInfo(m_yuvInf);
}

bool SGYuvVideoNode::setYuv(const unsigned char * yuv, unsigned int len)
{
    if (!yuv) return false;
    if (len >= 0 && len < m_yuvInf->m_len) return false;

    m_texMaterial->setYuv(yuv);
    return true;
}
