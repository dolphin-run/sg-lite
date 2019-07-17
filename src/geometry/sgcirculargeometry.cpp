#include "sgcirculargeometry.h"


//
struct CircularMesh
{
    int m_numVertices = 0;
    int m_latitude = 80;
    std::vector<VertexAsPointColor> m_vertices;

    CircularMesh()
    {
        m_numVertices = 0;

        for (int i = 0; i < m_latitude; i++)
        {
            float theta0 = 2 * 3.14159f * (float)(i) / m_latitude;
            float theta1 = 2 * 3.14159f * (float)(i + 1) / m_latitude;

            float x0 = 0.5f * cos(theta0);
            float y0 = 0.5f * sin(theta0);
            float x1 = 0.5f * cos(theta1);
            float y1 = 0.5f * sin(theta1);

            //triangle 1
            VertexAsPointColor pc1 = { x0,      y0,     0, 1,   1, 0, 0, 1 };
            VertexAsPointColor pc2 = { x0 / 3,  y0 / 3, 0, 1,   0, 1, 0, 1 };
            VertexAsPointColor pc3 = { x1,      y1,     0, 1,   1, 0, 0, 1 };
            VertexAsPointColor pc4 = { x1 / 3,  y1 / 3, 0, 1,   0, 1, 0, 1 };
            m_vertices.push_back(pc1);
            m_vertices.push_back(pc2);
            m_vertices.push_back(pc3);
            m_vertices.push_back(pc4);

            m_numVertices += 4;
        }
    }
};

SGCircularGeometry::SGCircularGeometry():
    SGGeometry(E_RenderTriangleStrip)
{
    static CircularMesh mesh;

    static VertexAttribute vas[2] = {
        VertexAttribute{ 0 * sizeof(float), 4 },
        VertexAttribute{ 4 * sizeof(float), 4 }
    };
    static VertexAttributeSet attrs = {
        320,
        8,
        2,
        vas,
        320 * 8 * sizeof(float),
        8 * sizeof(float)
    };

    m_vertices = mesh.m_vertices;
    m_attrs = &attrs;
    m_data = m_vertices.data();
}

void SGCircularGeometry::setInnerRadius(float rate)
{
    assert(0.f <= rate && rate <= 1.f);
    auto pc = dataAsPointColor();
    auto pc1 = pc + 1;
    for (int i = 0; i < m_attrs->vnum; i += 2)
    {
        pc1->x = pc->x*rate;
        pc1->y = pc->y*rate;
        pc += 2;
        pc1 += 2;
    }
}

void SGCircularGeometry::setColor(const Color & c)
{
    setColor(c, c);
}

void SGCircularGeometry::setColor(const Color & inside, const Color & outside)
{
    auto pc = dataAsPointColor();
    //outside
    for (int i = 0; i < m_attrs->vnum; i += 2)
    {
        pc->r = outside.r / 255.f;
        pc->g = outside.g / 255.f;
        pc->b = outside.b / 255.f;
        pc += 2;
    }

    pc = dataAsPointColor();
    pc++;
    //inside
    for (int i = 1; i < m_attrs->vnum; i += 2)
    {
        pc->r = inside.r / 255.f;
        pc->g = inside.g / 255.f;
        pc->b = inside.b / 255.f;
        pc += 2;
    }
}
