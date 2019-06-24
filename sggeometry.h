#ifndef SGGEOMETRY_H
#define SGGEOMETRY_H

#include <cstdint>
#include <sgstruct.h>
//store vertex data
//define how data should be explained

enum E_GeometryType {
    ColorRectangleGeometryType
};

struct VertexAsColorRectangle
{
    float x, y, z, w;
    float r, g, b, a;
};

class SGGeometry
{
public:
    SGGeometry();
    virtual ~SGGeometry();
    void* data();
    VertexAsColorRectangle* dataAsColorRectangle();
    size_t size();
    size_t verticeSize();
    size_t verticeNum();

protected:
    void *m_data = nullptr;
    size_t m_size = 0;
    size_t m_vnum = 0;
    size_t m_vsize = 0;
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

#endif // SGGEOMETRY_H