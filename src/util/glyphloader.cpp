#include "glyphloader.h"


GlyphInfo GlyphLoader::m_invaildCharInfo;

GlyphLoader::GlyphLoader() :m_bLoaded(false)
{
}

GlyphLoader::~GlyphLoader()
{
    // Destroy FreeType once we're finished
    if (m_bLoaded)
    {
        FT_Done_Face(m_ftFace);
        FT_Done_FreeType(m_ftLibrary);
    }
}

void GlyphLoader::load(const char *font_file, int w, int h)
{
    FT_Error err = FT_Init_FreeType(&m_ftLibrary);
    if (err) return;

    //load font
    err = FT_New_Face(m_ftLibrary, font_file, 0, &m_ftFace);
    if (err) return;

    //
    FT_Select_Charmap(m_ftFace, FT_ENCODING_UNICODE);

    FT_Set_Pixel_Sizes(m_ftFace, w, h);

    m_bLoaded = true;
}

void GlyphLoader::unload()
{
}

GlyphInfo GlyphLoader::loadChar(wchar_t ch)
{
    assert(m_bLoaded);

    //load char will remove previous glyph
    if (FT_Load_Char(m_ftFace, ch, FT_LOAD_RENDER))
    {
        return m_invaildCharInfo;
    }
    FT_Render_Glyph(m_ftFace->glyph, FT_RENDER_MODE_NORMAL);

    //package the glyph info.
    GlyphInfo inf;
    inf.ch = ch;
    inf.width = m_ftFace->glyph->bitmap.width;
    inf.height = m_ftFace->glyph->bitmap.rows;
    inf.leftBearing = m_ftFace->glyph->bitmap_left;
    inf.topBearing = GLYPH_PIXEL_SIZE - m_ftFace->glyph->bitmap_top;
    inf.advance = m_ftFace->glyph->advance.x >> 6;
    inf.buffer = m_ftFace->glyph->bitmap.buffer;

    return inf;
}