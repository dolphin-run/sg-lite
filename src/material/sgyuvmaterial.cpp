#include "sgyuvmaterial.h"

class SGYuvMaterialShader : public SGMaterialShader
{
public:
    SGYuvMaterialShader(const SGYuvMaterial *parent) : m_parent(parent) { build(); }
    ~SGYuvMaterialShader() {}

    virtual void updateState(const glm::mat4 &projection) override
    {
        m_program.setMat4(m_matrixUniform, projection);

        m_program.setInt(m_texYUniform, 0);
        m_program.setInt(m_texUUniform, 1);
        m_program.setInt(m_texVUniform, 2);

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
            "uniform sampler2D tex_y;\n"
            "uniform sampler2D tex_u;\n"
            "uniform sampler2D tex_v;\n"
            "void main() {\n"
                "vec3 yuv;\n"
                "vec3 rgb;\n"
                "yuv.x = texture(tex_y, tex).r;\n"
                "yuv.y = texture(tex_u, tex).r - 0.5;\n"
                "yuv.z = texture(tex_v, tex).r - 0.5;\n"
                "\n"
                "//yuv420p\n"
                "rgb = mat3(1.0, 1.0, 1.0,\n"
                "    0.0, -0.395, 2.032,\n"
                "    1.140, -0.581, 0.0) * yuv;\n"
                "gl_FragColor = vec4(rgb, 1.0); \n"
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
        m_texYUniform = m_program.uniformLocation("tex_y");
        m_texUUniform = m_program.uniformLocation("tex_u");
        m_texVUniform = m_program.uniformLocation("tex_v");

        return true;
    }
private:
    unsigned m_matrixUniform;
    unsigned m_texYUniform;
    unsigned m_texUUniform;
    unsigned m_texVUniform;
    const SGYuvMaterial *m_parent;
};

SGYuvMaterial::SGYuvMaterial()
{
}

E_MaterialType SGYuvMaterial::type() const
{
    return E_MaterialType::YuvMaterialType;
}

SGMaterialShader * SGYuvMaterial::createShader() const
{
    return new SGYuvMaterialShader(this);
}

void SGYuvMaterial::setYuv(const unsigned char * src)
{
    m_tex.add(Image(1920, 540, src, Image::ImageFormat::Format_RGB));
}

SGTexture * SGYuvMaterial::texture() const
{
    return &m_tex;
}
