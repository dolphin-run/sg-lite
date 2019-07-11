#include "sgcolormaterial.h"


class SGFlatColorMaterialShader : public SGMaterialShader
{
public:
    SGFlatColorMaterialShader() { build(); }
    ~SGFlatColorMaterialShader() {}

    virtual void updateState(const glm::mat4 &projection) override
    {
        m_program.setUniformValue(m_matrixUniform, glmtoqmat(projection));
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
    
    virtual char const* const* attributeNames() const override
    {
        static const char* const attr[] = { "posAttr", "colAttr", 0 };
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
};


E_MaterialType SGFlatColorMaterial::type() const
{
    return E_MaterialType::FlatColorMaterialType;
}

SGMaterialShader *SGFlatColorMaterial::createShader() const
{
    return new SGFlatColorMaterialShader;
}
