#include "sggeometry.h"

SGGeometry::SGGeometry()
{

}

SGGeometry::~SGGeometry()
{
}

VertexAsPointColor * SGGeometry::dataAsPointColor()
{
    return (VertexAsPointColor*)m_data;
}

VertexAsPointTexture * SGGeometry::dataAsPointTexture()
{
    return (VertexAsPointTexture*)m_data;
}
