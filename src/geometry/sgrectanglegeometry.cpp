#include "sgrectanglegeometry.h"


SGColorRectangleGeometry::SGColorRectangleGeometry() :
    SGGeometry(E_RenderTriangleStrip)
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
        pt->a = 1.f;// c.a / 255.f;
        pt++;
    }
}

SGTextureRectangleGeometry::SGTextureRectangleGeometry() :
    SGGeometry(E_RenderTriangleStrip)
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

//text geometry
SGTextRectangleGeometry::SGTextRectangleGeometry(int n) :
    SGGeometry(E_RenderTriangles)
{
    static VertexAttribute vas[2] = {
        VertexAttribute{ 0 * sizeof(float), 4 },
        VertexAttribute{ 4 * sizeof(float), 4 }
    };

    attrs.vstride = 8;
    attrs.vparanum = 2;
    attrs.vas = vas;
    attrs.vsize = 8 * sizeof(float);

    m_attrs = &attrs;

    resize(n);
}

SGTextRectangleGeometry::~SGTextRectangleGeometry()
{
    if (vertices) delete[] vertices;
}

void SGTextRectangleGeometry::resize(int n)
{
    num = n;

    attrs.vnum = 6 * num;
    attrs.size = attrs.vnum*attrs.vsize;

    if (vertices) delete[] vertices;

    vertices = new float[attrs.vnum * attrs.vstride];
    memset(vertices, 0, attrs.size);

    m_data = vertices;
}

void SGTextRectangleGeometry::setCoord(int idx, const RectF & rec)
{
    assert(idx < num);
    setCoord(idx, rec.tl.x, rec.tl.y, rec.br.x, rec.br.y);
}

void SGTextRectangleGeometry::setCoord(int idx, float left, float top, float right, float buttom)
{
    assert(idx < num);

    auto pt = dataAsPointTexture(idx * 6);
    //translate to gl rendering coordinate

    pt[0].x = right;
    pt[0].y = buttom;
    pt[0].z = 0;
    pt[0].w = 1;
    pt[1].x = left;
    pt[1].y = buttom;
    pt[1].z = 0;
    pt[1].w = 1;
    pt[2].x = right;
    pt[2].y = top;
    pt[2].z = 0;
    pt[2].w = 1;
    pt[3].x = left;
    pt[3].y = top;
    pt[3].z = 0;
    pt[3].w = 1;

    pt[4].x = left;
    pt[4].y = buttom;
    pt[4].z = 0;
    pt[4].w = 1;
    pt[5].x = right;
    pt[5].y = top;
    pt[5].z = 0;
    pt[5].w = 1;
}

void SGTextRectangleGeometry::setTexCoord(int idx, const RectF & rec)
{
    assert(idx < num);
    setTexCoord(idx, rec.tl.x, rec.tl.y, rec.br.x, rec.br.y);
}

void SGTextRectangleGeometry::setTexCoord(int idx, float left, float top, float right, float buttom)
{
    assert(idx < num);

    auto pt = dataAsPointTexture(idx*6);
    //translate to gl rendering coordinate
    pt[0].vx = right;
    pt[0].vy = buttom;
    pt[1].vx = left;
    pt[1].vy = buttom;
    pt[2].vx = right;
    pt[2].vy = top;
    pt[3].vx = left;
    pt[3].vy = top;

    pt[4].vx = left;
    pt[4].vy = buttom;
    pt[5].vx = right;
    pt[5].vy = top;
}