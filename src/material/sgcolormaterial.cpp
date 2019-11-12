#include "sgcolormaterial.h"


class SGFlatColorMaterialShader : public SGMaterialShader
{
public:
    SGFlatColorMaterialShader() { build(); }
    ~SGFlatColorMaterialShader() {}

    virtual void updateState(const float *mat4, const SGNodeState *stat) override
    {
        m_program.setMat4(m_matrixUniform, mat4);
        m_program.setFloat(m_opacityUniform, stat->opacity);
    }

    virtual const char* vertexShader() const override
    {
        return  "attribute vec4 posAttr;\n"
            "attribute vec4 colAttr;\n"
            "varying vec4 color;\n"
            "uniform mat4 matrix;\n"
            "void main() {\n"
            "   color = colAttr;\n"
            "   gl_Position = matrix * posAttr;\n"
            "}\n";
    }

    virtual const char* fragmentShader() const override
    {
        return  "varying vec4 color;\n"
            "uniform float opacity;\n"
            "void main() {\n"
            "   gl_FragColor = vec4(color.rgb, color.a*opacity);\n"
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
        m_opacityUniform = m_program.uniformLocation("opacity");

        return true;
    }
private:
    unsigned m_matrixUniform;
    unsigned m_opacityUniform;
};


E_MaterialType SGFlatColorMaterial::type() const
{
    return E_MaterialType::FlatColorMaterialType;
}

SGMaterialShader *SGFlatColorMaterial::createShader() const
{
    return new SGFlatColorMaterialShader;
}
