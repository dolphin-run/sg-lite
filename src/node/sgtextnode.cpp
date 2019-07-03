
#include "sgtextnode.h"
#include <QStandardPaths>


class GlyphLoader
{
public:
    GlyphLoader();
    ~GlyphLoader();

    void load(const char* font_file, int w, int h);
    void unload();

    Image loadChar(wchar_t ch);
    const WordInfo& retrieve(wchar_t ch);
    
private:
    FT_Library m_ftLibrary;
    FT_Face    m_ftFace;
    bool m_bLoaded;

    std::map<wchar_t, WordInfo> m_mapWordInfo;
};

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

    //加载一个字体,取默认的Face,一般为Regualer
    err = FT_New_Face(m_ftLibrary, font_file, 0, &m_ftFace);
    if (err) return;

    //选择字符表
    FT_Select_Charmap(m_ftFace, FT_ENCODING_UNICODE);

    FT_Set_Pixel_Sizes(m_ftFace, w, h);

    m_bLoaded = true;
}

void GlyphLoader::unload()
{
}

Image GlyphLoader::loadChar(wchar_t ch)
{
    if (!m_bLoaded) return Image();
    /* 装载字形图像到字形槽（将会抹掉先前的字形图像） */
    if (FT_Load_Char(m_ftFace, ch, FT_LOAD_RENDER))
    {
        return Image();
    }
    FT_Render_Glyph(m_ftFace->glyph, FT_RENDER_MODE_NORMAL);
    Image img(m_ftFace->glyph->bitmap.width,
        m_ftFace->glyph->bitmap.rows,
        m_ftFace->glyph->bitmap.buffer,
        Image::Format_Alpha);

    // Now store character for later use
    WordInfo character;
    character.m_chaID = ch;
    character.m_width = m_ftFace->glyph->bitmap.width;
    character.m_height = m_ftFace->glyph->bitmap.rows;
    character.m_leftBearing = m_ftFace->glyph->bitmap_left;
    character.m_topBearing = m_ftFace->glyph->bitmap_top;
    character.m_advance = m_ftFace->glyph->advance.x >> 6;
    character.m_widErrSemi = (WORDLOADER_PIXEL - character.m_advance) / 2.f;

    m_mapWordInfo.insert(std::pair<wchar_t, WordInfo>(ch, character));

    return img;
}

const WordInfo & GlyphLoader::retrieve(wchar_t ch)
{
    return m_mapWordInfo.at(ch);
}

static GlyphLoader g_glyphLoader;

void GlyphLoader_initialize()
{
    static bool loaded = false;
    if (loaded) return;
    loaded = true;

    QString fontPath = QStandardPaths::writableLocation(QStandardPaths::FontsLocation) + "/msyh.ttf";

    g_glyphLoader.load(fontPath.toLocal8Bit(), WORDLOADER_WORD_PIXEL, WORDLOADER_WORD_PIXEL);
}


SGTextNode::SGTextNode(SGNode * parent) :
    SGNode(E_NodeType::ImageNodeType, parent)
{
    GlyphLoader_initialize();
}

void SGTextNode::addLineText(const std::wstring & s, size_t first, size_t last)
{
    assert(s.size());
    assert(first < last);

    LineRenderInfo rendinf;
    int cx = 0;
    for(auto ch: s)
    {
        const WordInfo &inf = g_glyphLoader.retrieve(ch);
        //cx += inf->m_width + inf->m_leftBearing;
        cx += inf.m_advance >> 6;
        rendinf.m_lineInf.push_back(&inf);
    }
    rendinf.m_wordHPixSum = cx;
    rendinf.m_startHPix = cx / 2 - WORDLOADER_PIXEL / 2;
    m_glyphLines.push_back(rendinf);
}

void SGTextNode::setText(const std::wstring & txt)
{
    if (txt.empty()) return;

    size_t start = 0;
    while(1)
    {
        size_t pos = txt.find(L'\n', start); 
        addLineText(txt, start, pos);
        if (pos != std::wstring::npos)
        {
            start = pos + 1;
        }
    }
}
