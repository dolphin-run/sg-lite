#ifndef SGRECTANGLEGEOMETRY_H
#define SGRECTANGLEGEOMETRY_H

#include "sggeometry.h"

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
    void setTexCoord(const RectF &rec);
    void setTexCoord(float top, float left, float right, float buttom);
private:
    float vertices[32] = {
        0.5,  0.5, 0, 1,   1.0f, 1.0f, 0.0f, 1.0f,/*topright*/
        -0.5,  0.5, 0, 1,   0.0f, 1.0f, 0.0f, 1.0f,/*topleft*/
        0.5, -0.5, 0, 1,   1.0f, 0.0f, 0.0f, 1.0f,/*buttomright*/
        -0.5, -0.5, 0, 1,   0.0f, 0.0f, 0.0f, 1.0f/*buttomleft*/
    };
};


class SGTextRectangleGeometry : public SGGeometry
{
public:
    SGTextRectangleGeometry(int n = 1);
    ~SGTextRectangleGeometry();

    void resize(int n);
    void setCoord(int idx, const RectF &rec);
    void setCoord(int idx, float left, float top, float right, float buttom);
    void setTexCoord(int idx, const RectF &rec);
    void setTexCoord(int idx, float left, float top, float right, float buttom);
private:
    VertexAttributeSet attrs;
    float *vertices = nullptr;
    size_t num = 0;
};

#endif // SGRECTANGLEGEOMETRY_H
