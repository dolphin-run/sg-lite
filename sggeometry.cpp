#include "sggeometry.h"

SGGeometry::SGGeometry()
{

}

SGGeometry::~SGGeometry()
{
}

void* SGGeometry::data()
{
    return m_data;
}

VertexAsColorRectangle * SGGeometry::dataAsColorRectangle()
{
    return (VertexAsColorRectangle *)m_data;
}

size_t SGGeometry::size()
{
    return m_size;
}

size_t SGGeometry::verticeSize()
{
    return m_vsize;
}

size_t SGGeometry::verticeNum()
{
    return m_vnum;
}

SGColorRectangleGeometry::SGColorRectangleGeometry()
{
    m_vnum = 4;
    m_vsize = 8 * sizeof(float);

    m_data = vertices;
    m_size = m_vnum*m_vsize;
}

void SGColorRectangleGeometry::setColor(const Color & c)
{
    for (int i = 0; i < 4; i++)
    {
        vertices[8 * i + 4] = c.r / 255.f;
        vertices[8 * i + 5] = c.g / 255.f;
        vertices[8 * i + 6] = c.b / 255.f;
        vertices[8 * i + 7] = 1.f;
    }
}
