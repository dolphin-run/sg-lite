#ifndef SGTEXTNODE_P_H
#define SGTEXTNODE_P_H

#include "util/sgglyph.h"
#include <string>

class SGGlyphMaterial;
class SGTextRectangleGeometry;
class SGTextNodePrivate
{
    SG_DECLARE_PUBLIC(SGTextNode);
protected:
    SGTextNodePrivate();

    Size setText(const std::wstring & txt);
    void addLineText(const std::wstring & s, size_t first, size_t last);
    void reAlign();

private:
    Color m_color; 
    HAlignment m_halignment;
    VAlignment m_valignment;
    int m_boundingWidth = 0;
    int m_boundingHeight = 0;

    std::vector<LineRenderInfo> m_glyphLines;
    SGGlyphMaterial *m_glyphMaterial = nullptr;
    SGTextRectangleGeometry *m_textGeometry = nullptr;
};

#endif // SGTEXTNODE_P_H
