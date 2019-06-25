#include "sgmaterial.h"


void SGMaterialShader::compile()
{
    m_program.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShader());
    m_program.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShader());
    if(!m_program.link())
        qDebug() << "SGMaterialShader Compile Error." << m_program.log();
}

class SGFlatColorMaterialShader : public SGMaterialShader
{
public:
    SGFlatColorMaterialShader() { build(); }
    ~SGFlatColorMaterialShader() {}

    virtual void updateState(const QMatrix4x4 &projection) override
    {
        m_program.setUniformValue(m_matrixUniform, projection);
    }

    virtual const char* vertexShader() const override
    {
        return  "attribute highp vec4 posAttr;\n"
            "attribute lowp vec4 colAttr;\n"
            "varying lowp vec4 col;\n"
            "uniform highp mat4 matrix;\n"
            "void main() {\n"
            "   col = colAttr;\n"
            "   gl_Position = matrix * posAttr;\n"
            "}\n";
    }

    virtual const char* fragmentShader() const override
    {
        return  "varying lowp vec4 col;\n"
            "void main() {\n"
            "   gl_FragColor = col;\n"
            "}\n";
    }
    
    bool build()
    {
        compile();
        m_posAttr = m_program.attributeLocation("posAttr");
        m_colAttr = m_program.attributeLocation("colAttr");
        m_matrixUniform = m_program.uniformLocation("matrix");

        return true;
    }
private:
    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;
};

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

E_MaterialType SGFlatColorMaterial::type() const
{
    return E_MaterialType::FlatColorMaterialType;
}

SGMaterialShader *SGFlatColorMaterial::createShader() const
{
    return new SGFlatColorMaterialShader;
}

E_MaterialType SGCircularMaterial::type() const
{
    return E_MaterialType::FlatColorMaterialType;
}

SGMaterialShader *SGCircularMaterial::createShader() const
{
    return new SGFlatColorMaterialShader;
}

E_MaterialType SGTextureMaterial::type() const
{
    return E_MaterialType::TextureMaterialType;
}

SGMaterialShader * SGTextureMaterial::createShader() const
{
    return new SGTextureMaterialShader;
}
