#ifndef SGCIRCULARNODE_H
#define SGCIRCULARNODE_H

#include "sgnode.h"

class SGCircularGeometry;
class SG_DECL_EXPORT SGCircularNode : public SGNode
{
    SG_DISABLE_COPY(SGCircularNode);
public:
    SGCircularNode(SGNode* parent = nullptr);
    void setInnerRadius(float rate);
    void setColor(const Color &c);
    void setColor(const Color &inside, const Color &outside);

private:
    Color m_colorInside;
    Color m_colorOutside;
    SGCircularGeometry *m_colorGeometry = nullptr;
};

#endif // SGCIRCULARNODE_H
