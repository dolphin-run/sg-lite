#include "sggeometry.h"

SGGeometry::SGGeometry(E_RenderType type):
    m_rtype(type)
{

}

SGGeometry::~SGGeometry()
{
}

VertexAsPointColor * SGGeometry::dataAsPointColor()
{
    return (VertexAsPointColor*)m_data;
}

VertexAsPointTexture * SGGeometry::dataAsPointTexture(size_t offset)
{
    return offset + (VertexAsPointTexture*)m_data;
}
