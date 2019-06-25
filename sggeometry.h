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
    VertexAttributeSet *m_attrs = nullptr;
    void *m_data;
};

class SGColorRectangleGeometry : public SGGeometry
{
public:
    SGColorRectangleGeometry();
    void setColor(const Color &c);
private:
    float vertices[32] = {
        0.5,  0.5, 0, 1,   1.0f, 0.0f, 0.0f, 1.0f,/*topright*/
        -0.5,  0.5, 0, 1,   0.0f, 1.0f, 0.0f, 1.0f,/*topleft*/
        0.5, -0.5, 0, 1,   0.0f, 0.0f, 1.0f, 1.0f,/*buttomright*/
        -0.5, -0.5, 0, 1,   1.0f, 1.0f, 1.0f, 1.0f/*buttomleft*/
    };
};

class SGTextureRectangleGeometry : public SGGeometry
{
public:
    SGTextureRectangleGeometry();
    void setTexCoord(float top, float left, float right, float buttom);
private:
    float vertices[32] = {
        0.5,  0.5, 0, 1,   1.0f, 1.0f, 0.0f, 1.0f,/*topright*/
        -0.5,  0.5, 0, 1,   0.0f, 1.0f, 0.0f, 1.0f,/*topleft*/
        0.5, -0.5, 0, 1,   1.0f, 0.0f, 0.0f, 1.0f,/*buttomright*/
        -0.5, -0.5, 0, 1,   0.0f, 0.0f, 0.0f, 1.0f/*buttomleft*/
    };
};

class SGColorCircularGeometry : public SGGeometry
{
public:
    SGColorCircularGeometry();
    //void setRing(float rate);
    void setColor(const Color &c);
    void setColor(const Color &inside, const Color &outside);
private:
    std::vector<VertexAsPointColor> m_vertices;
};
#endif // SGGEOMETRY_H