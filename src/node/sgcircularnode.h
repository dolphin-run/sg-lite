#ifndef SGCIRCULARNODE_H
#define SGCIRCULARNODE_H

#include "sgnode.h"

class SGCircularGeometry;
class SGCircularNode : public SGNode
{
public:
    SGCircularNode(SGNode* parent = nullptr);
    void setColor(int32_t c);

private:
    Color m_color;
    SGCircularGeometry *m_colorGeometry = nullptr;
};

#endif // SGCIRCULARNODE_H
