#include "sgdynimagematerial.h"
#include <algorithm>


class SGDynImageMaterialShader : public SGMaterialShader
{
public:
    SGDynImageMaterialShader(const SGDynImageMaterial *parent) : m_parent(parent) { build(); }
    ~SGDynImageMaterialShader() {}

    virtual void updateState(const glm::mat4 &projection) override
    {
        m_program.setMat4(m_matrixUniform, projection);

        m_program.setInt(m_texUniform, 0);

        m_parent->texture()->bind();
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
        m_texUniform = m_program.uniformLocation("uSampler");

        return true;
    }
private:
    unsigned m_matrixUniform;
    unsigned m_texUniform;
    const SGDynImageMaterial *m_parent;
};

SGDynImageMaterial::SGDynImageMaterial()
{
}

E_MaterialType SGDynImageMaterial::type() const
{
    return E_MaterialType::DynImageMaterialType;
}

SGMaterialShader * SGDynImageMaterial::createShader() const
{
    return new SGDynImageMaterialShader(this);
}

void SGDynImageMaterial::setRgb(const unsigned char * src)
{
    auto const *ptr = src;
    m_tex.update(0, Image(1920, 1080, ptr, Image::Format_RGB));
}

SGTexture * SGDynImageMaterial::texture() const
{
    return &m_tex;
}
