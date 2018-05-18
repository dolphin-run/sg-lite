#ifndef GLYPHLOADER_H
#define GLYPHLOADER_H

#include "sgstruct.h"
//font loader
#include "stb/stb_truetype.h"

#include "sgglyph.h"

class GlyphLoader
{
public:
    GlyphLoader();
    ~GlyphLoader();

    bool load(const char* font_file, int w, int h);
    void unload();

    GlyphInfo loadChar(wchar_t ch);
    int getLineGap() const;

private:
    stbtt_fontinfo ttf_font;
    float ttf_pxScale;
    int ttf_ascent, ttf_descent, ttf_linegap;

    bool m_bLoaded;

    unsigned char *ttf_buffer;
    unsigned int ttf_buffsize = 1 << 26;
    static GlyphInfo m_invaildCharInfo;
};


#endif // GLYPHLOADER_H
