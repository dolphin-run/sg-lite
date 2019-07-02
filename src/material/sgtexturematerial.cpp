#include "sgtexturematerial.h"


std::unordered_map<std::string, unsigned> g_nameMapToKey;
SGImageTexture g_imgTex;

class SGTextureMaterialShader : public SGMaterialShader
{
public:
    SGTextureMaterialShader(const SGTextureMaterial *parent) : m_parent(parent) { build(); }
    ~SGTextureMaterialShader() {}

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
            "   gl_FragColor = texture2D(uSampler, vec2(tex.x, tex.y)); \n"
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
    const SGTextureMaterial *m_parent;
};

SGTextureMaterial::SGTextureMaterial()
{
    m_tex = &g_imgTex;
}

E_MaterialType SGTextureMaterial::type() const
{
    return E_MaterialType::TextureMaterialType;
}

SGMaterialShader * SGTextureMaterial::createShader() const
{
    return new SGTextureMaterialShader(this);
}

const RectF& SGTextureMaterial::setImage(const char * path)
{
    unsigned uid = 0;
    if (g_nameMapToKey.count(path))
    {
        uid = g_nameMapToKey.at(path);
    }
    else
    {
        uid = g_imgTex.add(path);
    }

    if (uid)
    {
        //only add valid file to map
        g_nameMapToKey.emplace(std::make_pair(path, uid));
    }

    return g_imgTex.retrieve(uid);
}

SGTexture * SGTextureMaterial::texture() const
{
    return m_tex;
}
