#ifndef SGCIRCULARGEOMETRY_H
#define SGCIRCULARGEOMETRY_H

#include "sggeometry.h"
#include <vector>

class SGCircularGeometry : public SGGeometry
{
public:
    SGCircularGeometry();
    void setInnerRadius(float rate);//rate to outer radius
    void setColor(const Color &c);
    void setColor(const Color &inside, const Color &outside);
private:
    std::vector<VertexAsPointColor> m_vertices;
};
#endif // SGCIRCULARGEOMETRY_H
