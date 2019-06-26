#ifndef SGCIRCULARGEOMETRY_H
#define SGCIRCULARGEOMETRY_H

#include "sggeometry.h"

class SGCircularGeometry : public SGGeometry
{
public:
    SGCircularGeometry();
    //void setRing(float rate);
    void setColor(const Color &c);
    void setColor(const Color &inside, const Color &outside);
private:
    std::vector<VertexAsPointColor> m_vertices;
};
#endif // SGCIRCULARGEOMETRY_H
