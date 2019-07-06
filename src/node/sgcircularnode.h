#ifndef SGCIRCULARNODE_H
#define SGCIRCULARNODE_H

#include "sgnode.h"

class SGCircularGeometry;
class SGCircularNode : public SGNode
{
public:
    SGCircularNode(SGNode* parent = nullptr);
    void setColor(const Color &c);
    void setColor(const Color &inside, const Color &outside);

private:
    Color m_colorInside;
    Color m_colorOutside;
    SGCircularGeometry *m_colorGeometry = nullptr;
};

#endif // SGCIRCULARNODE_H
