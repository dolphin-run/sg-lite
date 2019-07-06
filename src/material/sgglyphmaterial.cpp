#include "sgglyphmaterial.h"
#include "texture/sgglyphtexture.h"

//font loader
#include "ft2build.h"
#include "freetype/freetype.h"
#include "freetype/ftglyph.h"
#include "freetype/ftoutln.h"
#include "freetype/fttrigon.h"

#include <QStandardPaths>


class GlyphLoader
{
public:
    GlyphLoader();
    ~GlyphLoader();

    void load(const char* font_file, int w, int h);
    void unload();

    GlyphInfo loadChar(wchar_t ch);

private:
    FT_Library m_ftLibrary;
    FT_Face    m_ftFace;
    bool m_bLoaded;

    static GlyphInfo m_invaildCharInfo;
};


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

static GlyphLoader g_glyphLoader;

void GlyphLoader_initialize()
{
    static bool loaded = false;
    if (loaded) return;
    loaded = true;

    QString fontPath = QStandardPaths::writableLocation(QStandardPaths::FontsLocation) + "/msyh.ttc";

    g_glyphLoader.load(fontPath.toLocal8Bit(), GLYPH_PIXEL_SIZE, GLYPH_PIXEL_SIZE);
}

///////


std::unordered_map<wchar_t, GlyphInfo> g_mapGlyphs;
SGGlyphTexture g_glyphTex;

class SGGlyphMaterialShader : public SGMaterialShader
{
public:
    SGGlyphMaterialShader(const SGGlyphMaterial *parent) : m_parent(parent) { build(); }
    ~SGGlyphMaterialShader() {}

    virtual void updateState(const QMatrix4x4 &projection) override
    {
        m_program.setUniformValue(m_matrixUniform, projection);

        m_parent->texture()->bind();
    }

    virtual const char* vertexShader() const override
    {
        return  "attribute highp vec4 posAttr;\n"
            "attribute highp vec4 texAttr;\n"
            "varying highp vec4 tex;\n"
            "uniform highp mat4 matrix;\n"
            "void main() {\n"
            "   tex = texAttr;\n"
            "   gl_Position = matrix * posAttr;\n"
            "}\n";
    }

    virtual const char* fragmentShader() const override
    {
        return  "varying highp vec4 tex;\n"
            "uniform sampler2D uSampler;\n"
            "void main() {\n"
            "   float dist = texture2D(uSampler, vec2(tex.x, tex.y)).r; \n"
            "   float alpha = smoothstep(0.7, 0.8, dist); \n"
            "   gl_FragColor = vec4(1.0, 0.0, 0.0, alpha); \n"
            "}\n";
    }

    virtual char const* const* attributeNames() const override
    {
        static const char* const attr[] = { "posAttr", "texAttr", 0 };
        return attr;
    }

    bool build()
    {
        compile();
        m_matrixUniform = m_program.uniformLocation("matrix");

        return true;
    }
private:
    GLuint m_matrixUniform;
    const SGGlyphMaterial *m_parent;
};

SGGlyphMaterial::SGGlyphMaterial()
{
    m_tex = &g_glyphTex;
}

E_MaterialType SGGlyphMaterial::type() const
{
    return E_MaterialType::GlyphMaterialType;
}

SGMaterialShader * SGGlyphMaterial::createShader() const
{
    return new SGGlyphMaterialShader(this);
}

const GlyphInfo& SGGlyphMaterial::setGlyph(wchar_t ch)
{
    if (!g_mapGlyphs.count(ch))
    {
        //create it.
        GlyphInfo inf = g_glyphLoader.loadChar(ch);

        Image img(inf.width, inf.height, inf.buffer, Image::Format_Alpha);
        unsigned uid = g_glyphTex.add(img);

        inf.tex = g_glyphTex.retrieve(uid);
        inf.buffer = nullptr;

        g_mapGlyphs.emplace(std::make_pair(ch, inf));
    }
    return g_mapGlyphs.at(ch);
}

SGTexture * SGGlyphMaterial::texture() const
{
    return m_tex;
}
