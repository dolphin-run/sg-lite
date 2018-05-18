#include "glyphloader.h"
#include <cstdio>
#define STB_TRUETYPE_IMPLEMENTATION  // force following include to generate implementation
#include "stb/stb_truetype.h"

GlyphInfo GlyphLoader::m_invaildCharInfo;

GlyphLoader::GlyphLoader() :m_bLoaded(false)
{
    ttf_buffer = new unsigned char[ttf_buffsize];
}

GlyphLoader::~GlyphLoader()
{
    // Destroy FreeType once we're finished
    if (ttf_buffer)
    {
        delete []ttf_buffer;
        ttf_buffer = nullptr;
    }
}

bool GlyphLoader::load(const char *font_file, int w, int h)
{
    //load font
    FILE *fontfile = fopen(font_file, "rb");
    if (!fontfile) return false;

    auto realsize = fread(ttf_buffer, 1, ttf_buffsize, fontfile);
    assert(realsize < ttf_buffsize);

    stbtt_InitFont(&ttf_font, ttf_buffer, stbtt_GetFontOffsetForIndex(ttf_buffer, 0));

    //calculate for later using
    ttf_pxScale = stbtt_ScaleForPixelHeight(&ttf_font, h);
    stbtt_GetFontVMetrics(&ttf_font, &ttf_ascent, &ttf_descent, &ttf_linegap);

    
    m_bLoaded = true;
    return true;
}

void GlyphLoader::unload()
{
}

GlyphInfo GlyphLoader::loadChar(wchar_t ch)
{
    assert(m_bLoaded);

    int width, height;
    int advance, lsb, x0, y0, x1, y1;
    unsigned char *bitmap = stbtt_GetCodepointSDF(&ttf_font, ttf_pxScale, ch, 1, 127, 100, &width, &height, 0, 0);
    //unsigned char *bitmap = stbtt_GetCodepointBitmap(&ttf_font, ttf_pxScale, ttf_pxScale, ch, &width, &height, 0, 0);
    stbtt_GetCodepointHMetrics(&ttf_font, ch, &advance, &lsb);
    stbtt_GetCodepointBitmapBoxSubpixel(&ttf_font, ch, ttf_pxScale, ttf_pxScale, 0, 0, &x0, &y0, &x1, &y1);

    //maybe always zero, skip it.
    //int kern = 0;
    //if (ch1)
    //{
    //    kern = stbtt_GetCodepointKernAdvance(&ttf_font, ch, ch1);
    //}

    //package the glyph info.
    GlyphInfo inf;
    inf.ch = ch;
    inf.width = width;
    inf.height = height;
    inf.leftBearing = x0;
    inf.topBearing = ttf_ascent*ttf_pxScale + y0;
    inf.advance = (advance * ttf_pxScale);
    inf.buffer = bitmap;

    return inf;
}

int GlyphLoader::getLineGap() const
{
    return ttf_linegap*ttf_pxScale;
}
