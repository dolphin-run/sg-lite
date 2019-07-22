#ifndef SGTEXTNODE_H
#define SGTEXTNODE_H

#include "sgnode.h"
#include "util/sgglyph.h"

class SGGlyphMaterial;
class SGTextRectangleGeometry;
class SGTextNode : public SGNode
{
public:
    SGTextNode(SGNode* parent = nullptr);

    void setText(const std::wstring &txt);
    void setColor(const Color &color);

protected:
    void addLineText(const std::wstring & s, size_t first, size_t last);

private:
    Color m_color; 
    std::vector<LineRenderInfo> m_glyphLines;
    SGGlyphMaterial *m_glyphMaterial = nullptr;
    SGTextRectangleGeometry *m_textGeometry = nullptr;
};

#endif // SGTEXTNODE_H
