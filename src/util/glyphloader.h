#ifndef GLYPHLOADER_H
#define GLYPHLOADER_H

#include "sgstruct.h"
//font loader
#include "ft2build.h"
#include "freetype/freetype.h"
#include "freetype/ftglyph.h"
#include "freetype/ftoutln.h"
#include "freetype/fttrigon.h"

#include "sgglyph.h"

class GlyphLoader
{
public:
    GlyphLoader();
    ~GlyphLoader();

    bool load(const char* font_file, int w, int h);
    void unload();

    GlyphInfo loadChar(wchar_t ch);

private:
    FT_Library m_ftLibrary;
    FT_Face    m_ftFace;
    bool m_bLoaded;

    static GlyphInfo m_invaildCharInfo;
};


#endif // GLYPHLOADER_H
