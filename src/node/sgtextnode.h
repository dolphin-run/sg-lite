#ifndef SGTEXTNODE_H
#define SGTEXTNODE_H

#include "sgnode.h"
#include <string>

class SGGlyphMaterial;
class SGTextRectangleGeometry;
class SGTextNodePrivate;
class SG_DECL_EXPORT SGTextNode : public SGNode
{
    SG_DISABLE_COPY(SGTextNode);
    SG_DECLARE_PRIVATE(SGTextNode);
public:
    SGTextNode(SGNode* parent = nullptr);
    ~SGTextNode();

    void setText(const std::wstring &txt);
    void setColor(const Color &color);
};

#endif // SGTEXTNODE_H
