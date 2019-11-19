#include "sgglyphmaterial.h"
#include "texture/sggridimagetexture.h"

#include "util/glyphloader.h"
#include <stb/stb_image.h>


static std::unordered_map<wchar_t, GlyphInfo> g_mapGlyphs;
static SGGridImageTexture g_glyphTex(3072, 3072, Image::Format_Alpha, GLYPH_METRICS_SIZE, GLYPH_METRICS_SIZE);
static GlyphLoader g_msyhGlyph;
static void msyh_glyph_initialize()
{
    static bool loaded = false;
    if (loaded) return;
    loaded = true;

#ifdef _WIN32
    const char* fontPathList[] = {
        "C:/Windows/Fonts/msyh.ttc",
        "C:/Windows/Fonts/msyh.ttf",
        "C:/Windows/Fonts/simsun.ttc"
    };
#endif

#ifdef __linux__
    const char* fontPathList[] = {
        "/usr/share/fonts/truetype/freefont/FreeSerif.ttf"
    };
#endif

#ifdef __APPLE__
    const char* fontPathList[] = {
        "/System/Library/Fonts/STHeiti Light.ttc"
    };
#endif
    bool loadStat = false;
    for (auto fontPath : fontPathList)
    {
        loadStat = g_msyhGlyph.load(fontPath, GLYPH_PIXEL_SIZE, GLYPH_PIXEL_SIZE);
        if (loadStat) break;
    }
    assert(loadStat);
}

class SGGlyphMaterialShader : public SGMaterialShader
{
public:
    SGGlyphMaterialShader() { build(); }
    ~SGGlyphMaterialShader() {}

    virtual void updateState(const float *mat4, const SGNodeState *stat) override
    {
        m_program.setMat4(m_matrixUniform, mat4);
        m_program.setVec4(m_colorUniform, glm::vec4(stat->color.r / 255.f, stat->color.g / 255.f, stat->color.b / 255.f, 1.f));
        m_program.setFloat(m_opacityUniform, stat->opacity);
    }

    virtual const char* vertexShader() const override
    {
        return  "attribute vec4 posAttr;\n"
            "attribute vec4 texAttr;\n"
            "varying vec4 tex;\n"
            "uniform mat4 matrix;\n"
            "void main() {\n"
            "   tex = texAttr;\n"
            "   gl_Position = matrix * posAttr;\n"
            "}\n";
    }

    virtual const char* fragmentShader() const override
    {
        return  "varying vec4 tex;\n"
            "uniform vec4 color;\n"
            "uniform sampler2D uSampler;\n"
            "const float smoothing = 1.0/16.0;\n"
            "uniform float opacity;\n"
            "void main() {\n"
            "   float dist = texture2D(uSampler, vec2(tex.x, tex.y)).r; \n"
            //"   float alpha = smoothstep(0.5 - smoothing, 0.5 + smoothing, dist); \n"
            "   float width = fwidth(dist);\n"
            "   float alpha = clamp(smoothstep(0.5 - width, 0.5 + width, dist), 0.0, 1.0);\n"
            "   gl_FragColor = vec4(color.r, color.g, color.b, alpha*opacity); \n"
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
        m_colorUniform = m_program.uniformLocation("color");
        m_opacityUniform = m_program.uniformLocation("opacity");

        return true;
    }
private:
    unsigned m_matrixUniform;
    unsigned m_colorUniform;
    unsigned m_opacityUniform;
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
    return new SGGlyphMaterialShader;
}

const GlyphInfo& SGGlyphMaterial::setGlyph(wchar_t ch)
{
    if (!g_mapGlyphs.count(ch))
    {
        //create it.
        GlyphInfo inf = g_msyhGlyph.loadChar(ch);

        Image img(inf.width, inf.height, inf.buffer, Image::Format_Alpha);
        unsigned uid = g_glyphTex.add(img);
        free((void*)inf.buffer);

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

void SGGlyphMaterial::syncState()
{
    m_tex->bind();
}
