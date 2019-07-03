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

void SGTextureRectangleGeometry::setTexCoord(const RectF & rec)
{
    setTexCoord(rec.tl.x, rec.tl.y, rec.br.x, rec.br.y);
}

//use screen coordinate
void SGTextureRectangleGeometry::setTexCoord(float left, float top, float right, float buttom)
{
    auto pt = dataAsPointTexture();
    //translate to gl rendering coordinate
    pt[0].vx = right;
    pt[0].vy = buttom;
    pt[1].vx = left;
    pt[1].vy = buttom;
    pt[2].vx = right;
    pt[2].vy = top;
    pt[3].vx = left;
    pt[3].vy = top;
}

SGTextRectangleGeometry::SGTextRectangleGeometry(int n)
    : num(n)
{
    static VertexAttribute vas[2] = {
        VertexAttribute{ 0 * sizeof(float), 4 },
        VertexAttribute{ 4 * sizeof(float), 4 }
    };
    attrs.vnum = 4 * num + (num - 1) * 2;
    attrs.vstride = 8;
    attrs.vparanum = 2;
    attrs.vas = vas;
    attrs.vsize = 8 * sizeof(float);
    attrs.size = attrs.vnum*attrs.vsize;

    vertices = new float[attrs.vnum * attrs.vstride];

    m_attrs = &attrs;
    m_data = vertices;
}

SGTextRectangleGeometry::~SGTextRectangleGeometry()
{
}

void SGTextRectangleGeometry::setTexCoord(int idx, const RectF & rec)
{
    setTexCoord(idx, rec.tl.x, rec.tl.y, rec.br.x, rec.br.y);
}

void SGTextRectangleGeometry::setTexCoord(int idx, float top, float left, float right, float buttom)
{
    for (int i = 0; i < num; i++)
    {
        auto pt = dataAsPointTexture(i*6);
        //translate to gl rendering coordinate
        pt[0].vx = right;
        pt[0].vy = buttom;
        pt[1].vx = left;
        pt[1].vy = buttom;
        pt[2].vx = right;
        pt[2].vy = top;
        pt[3].vx = left;
        pt[3].vy = top;
    }
}
