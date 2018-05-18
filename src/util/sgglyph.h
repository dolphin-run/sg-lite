#ifndef SGGLYPH_H
#define SGGLYPH_H

#include "sgstruct.h"
#include <vector>

const int GLYPH_PIXEL_SIZE = 64;
const int GLYPH_LINE_SIZE = 64;
const int GLYPH_METRICS_SIZE = 96;

struct GlyphInfo
{
    wchar_t ch = 0;

    int width, height;//size
    int leftBearing, topBearing;//offset from baseline, we change the top to fit gl rendering
    int advance;//

    //only valid when loading char, we mean it...
    const unsigned char* buffer = nullptr;
    RectF tex;
};

struct LineRenderInfo
{
    std::vector<const GlyphInfo*> lineInf;
    //int m_startHPix;
    int width = 0;
};

#endif // SGGLYPH_H
