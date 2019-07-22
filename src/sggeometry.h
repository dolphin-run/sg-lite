#ifndef SGGEOMETRY_H
#define SGGEOMETRY_H

#include <cstdint>
#include "sgstruct.h"
#include <math.h>
//store vertex data
//define how data should be explained


enum E_RenderType {
    E_RenderTriangles = 0x0004,
    E_RenderTriangleStrip = 0x0005
};

struct VertexAsPointColor
{
    float x, y, z, w;
    float r, g, b, a;
};

struct VertexAsPointTexture
{
    float x, y, z, w;
    float vx, vy, vz, vw;
};

struct VertexAttribute
{
    int offset;//offset with float
    int size;//size with bytes
};
struct VertexAttributeSet
{
    //attribute
    int vnum;//vertex number
    int vstride;//float number of one vertex
    int vparanum;//parameter number of one vertex
    VertexAttribute *vas;//parameter info of each vertex

    //for convinience
    int size;//all data bytes
    int vsize;//size of one vertex
};

class SGGeometry
{
public:
    SGGeometry(E_RenderType type);
    virtual ~SGGeometry();
    VertexAsPointColor* dataAsPointColor();
    VertexAsPointTexture* dataAsPointTexture(size_t offset = 0);

    inline const VertexAttributeSet* attributes() const { return m_attrs; }
    inline void* data() { return m_data; }
    inline int size() { return m_attrs->size; }
    inline int verticeSize() { return m_attrs->vsize; }
    inline int verticeNum() { return m_attrs->vnum; }
    inline E_RenderType renderType() { return m_rtype; }

protected:
    const VertexAttributeSet *m_attrs = nullptr;
    void *m_data;
    E_RenderType m_rtype;
};

#endif // SGGEOMETRY_H
