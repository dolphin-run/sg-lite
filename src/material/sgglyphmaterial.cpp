#include "sgglyphmaterial.h"
#include "texture/sggridimagetexture.h"

#include "util/glyphloader.h"

#include <QStandardPaths>


static std::unordered_map<wchar_t, GlyphInfo> g_mapGlyphs;
static SGGridImageTexture g_glyphTex(2048, 2048, Image::Format_Alpha, 64, 64);
static GlyphLoader g_msyhGlyph;
static void msyh_glyph_initialize()
{
    static bool loaded = false;
    if (loaded) return;
    loaded = true;

    QString fontPath = QStandardPaths::writableLocation(QStandardPaths::FontsLocation) + "/msyh.ttc";

    g_msyhGlyph.load(fontPath.toLocal8Bit(), GLYPH_PIXEL_SIZE, GLYPH_PIXEL_SIZE);
}

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
    msyh_glyph_initialize();

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
        GlyphInfo inf = g_msyhGlyph.loadChar(ch);

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
