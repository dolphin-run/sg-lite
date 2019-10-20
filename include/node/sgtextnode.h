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

    void setAlignment(HAlignment ha, VAlignment va);
    void setHAlignment(HAlignment ha);
    void setVAlignment(VAlignment va);
    void setText(const std::wstring &txt);
    void setColor(const Color &color);
    void setPointSize(int point);

protected:
    virtual bool compare(const SGNode *node) const;
    virtual SGNodeState nodeState() const override;
};

#endif // SGTEXTNODE_H
