#include "sgtexturematerial.h"


class SGTextureMaterialShader : public SGMaterialShader
{
public:
    SGTextureMaterialShader() { build(); }
    ~SGTextureMaterialShader() {}

    virtual void updateState(const QMatrix4x4 &projection) override
    {
        m_program.setUniformValue(m_matrixUniform, projection);
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

    bool build()
    {
        compile();
        m_posAttr = m_program.attributeLocation("posAttr");
        m_texAttr = m_program.attributeLocation("texAttr");
        m_matrixUniform = m_program.uniformLocation("matrix");

        return true;
    }
private:
    GLuint m_posAttr;
    GLuint m_texAttr;
    GLuint m_matrixUniform;
};

SGTextureMaterial::SGTextureMaterial()
{
    m_tex = &m_imgTex;
}

E_MaterialType SGTextureMaterial::type() const
{
    return E_MaterialType::TextureMaterialType;
}

SGMaterialShader * SGTextureMaterial::createShader() const
{
    return new SGTextureMaterialShader;
}

const RectF& SGTextureMaterial::addImage(const char * path)
{
    unsigned uid = 0;
    if (m_nameMapToKey.count(path))
    {
        uid = m_nameMapToKey.at(path);
    }
    else
    {
        uid = m_imgTex.add(path);
    }

    if (uid)
    {
        //only add valid file to map
        m_nameMapToKey.emplace(std::make_pair(path, uid));
    }

    return m_imgTex.retrieve(uid);
}

SGTexture * SGTextureMaterial::texture() const
{
    return m_tex;
}
