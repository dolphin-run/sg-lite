#include "sgrectanglenode.h"

#include "geometry/sgrectanglegeometry.h"
#include "material/sgcolormaterial.h"
#include "material/sgtexturematerial.h"
#include "material/sgyuvmaterial.h"
#include "material/sgdynimagematerial.h"
#include "sgnode_p.h"

SGRectangleNode::SGRectangleNode(SGNode * parent):
    SGNode(E_NodeType::RectangleNodeType, parent)
{
    m_colorGeometry = new SGColorRectangleGeometry;
    setGeometry(m_colorGeometry);
    setMaterial(new SGFlatColorMaterial);
}

void SGRectangleNode::setColor(Color c)
{
    m_colorGeometry->setColor(c);
}

SGImageNode::SGImageNode(SGNode * parent) :
    SGNode(E_NodeType::ImageNodeType, parent)
{
    m_texMaterial = new SGTextureMaterial;
    m_texGeometry = new SGTextureRectangleGeometry;
    setGeometry(m_texGeometry);
    setMaterial(m_texMaterial);
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
    m_texMaterial = new SGYuvMaterial;
    m_texGeometry = new SGTextureRectangleGeometry;
    setGeometry(m_texGeometry);
    setMaterial(m_texMaterial);

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

SGRgbVideoNode::SGRgbVideoNode(SGNode * parent):
    SGNode(E_NodeType::DynImageNodeType, parent)
{
    m_texMaterial = new SGDynImageMaterial;
    m_texGeometry = new SGTextureRectangleGeometry;
    setGeometry(m_texGeometry);
    setMaterial(m_texMaterial);

    //set default size
    setPixelSize();
}

void SGRgbVideoNode::setPixelSize(int pxWid, int pxHei)
{
    assert(pxWid > 0 && pxHei > 0);
    m_texMaterial->setSize(pxWid, pxHei);
}

bool SGRgbVideoNode::setRgb(const unsigned char * yuv, unsigned int len)
{
    if (!yuv) return false;

    m_texMaterial->setRgb(yuv);
    return true;
}