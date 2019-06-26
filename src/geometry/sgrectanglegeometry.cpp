#include "sgrectanglegeometry.h"


SGColorRectangleGeometry::SGColorRectangleGeometry()
{
    static VertexAttribute vas[2] = {
        VertexAttribute{ 0 * sizeof(float), 4 },
        VertexAttribute{ 4 * sizeof(float), 4 }
    };
    static VertexAttributeSet attrs = {
        4,
        8,
        2,
        vas,
        4 * 8 * sizeof(float),
        8 * sizeof(float)
    };

    m_attrs = &attrs;
    m_data = vertices;
}

void SGColorRectangleGeometry::setColor(const Color & c)
{
    auto pt = dataAsPointColor();
    for (int i = 0; i < 4; i++)
    {
        pt->r = c.r / 255.f;
        pt->g = c.g / 255.f;
        pt->b = c.b / 255.f;
        pt->a = 1.f;
        pt++;
    }
}

SGTextureRectangleGeometry::SGTextureRectangleGeometry()
{
    static VertexAttribute vas[2] = {
        VertexAttribute{ 0 * sizeof(float), 4 },
        VertexAttribute{ 4 * sizeof(float), 4 }
    };
    static VertexAttributeSet attrs = {
        4,
        8,
        2,
        vas,
        4 * 8 * sizeof(float),
        8 * sizeof(float)
    };

    m_attrs = &attrs;
    m_data = vertices;
}

void SGTextureRectangleGeometry::setTexCoord(float top, float left, float right, float buttom)
{
    auto pt = dataAsPointTexture();
    pt[0].vx = right;
    pt[0].vy = top;
    pt[1].vx = left;
    pt[1].vy = top;
    pt[2].vx = right;
    pt[2].vy = buttom;
    pt[3].vx = left;
    pt[3].vy = buttom;
}
