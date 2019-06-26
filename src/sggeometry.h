#ifndef SGGEOMETRY_H
#define SGGEOMETRY_H

#include <cstdint>
#include <sgstruct.h>
#include <math.h>
//store vertex data
//define how data should be explained

enum E_GeometryType {
    ColorRectangleGeometryType
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
    SGGeometry();
    virtual ~SGGeometry();
    VertexAsPointColor* dataAsPointColor();
    VertexAsPointTexture* dataAsPointTexture();

    inline const VertexAttributeSet* attributes() const { return m_attrs; }
    inline void* data() { return m_data; }
    inline int size() { return m_attrs->size; }
    inline int verticeSize() { return m_attrs->vsize; }
    inline int verticeNum() { return m_attrs->vnum; }

protected:
    const VertexAttributeSet *m_attrs = nullptr;
    void *m_data;
};

#endif // SGGEOMETRY_H
