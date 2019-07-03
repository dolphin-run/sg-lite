#ifndef SGTEXTNODE_H
#define SGTEXTNODE_H

#include "../sgnode.h"
#include "ft2build.h"
#include "freetype/freetype.h"
#include "freetype/ftglyph.h"
#include "freetype/ftoutln.h"
#include "freetype/fttrigon.h"

const int WORDLOADER_PIXEL = 128 * 0.6;
const int WORDLOADER_WORD_PIXEL = WORDLOADER_PIXEL*0.8;
struct WordInfo
{
    wchar_t m_chaID;
    int m_idx;

    int m_width, m_height;    // Size of glyph
    int m_leftBearing, m_topBearing;  // Offset from baseline to left/top of glyph
    int m_advance;
    float m_widErrSemi;
};

struct LineRenderInfo
{
    std::vector<const WordInfo*> m_lineInf;
    int m_startHPix;
    int m_wordHPixSum;
};

class SGTextureMaterial;
class SGColorRectangleGeometry;
class SGTextureRectangleGeometry;
class SGTextNode : public SGNode
{
public:
    SGTextNode(SGNode* parent = nullptr);

    void setText(const std::wstring &txt);

protected:
    void addLineText(const std::wstring & s, size_t first, size_t last);

private:
    Color m_color; 
    std::list<LineRenderInfo> m_glyphLines;
    SGColorRectangleGeometry *m_colorGeometry=nullptr;
};

#endif // SGTEXTNODE_H
