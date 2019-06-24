#ifndef SGMATERIAL_H
#define SGMATERIAL_H

#include <QOpenGLShaderProgram>
#include "sgstruct.h"

//define shader used to render
enum E_MaterialType{
    FlatColorMaterialType
};

class SGMaterial
{
public:
    class SGMaterialType{};
    virtual ~SGMaterial();
    virtual const char* vertexShader() const = 0;
    virtual const char* fragmentShader() const = 0;
    virtual const E_MaterialType type() const = 0;
    void active();
    QOpenGLShaderProgram *m_program = nullptr;

protected:
    bool compile();
    virtual bool build() = 0;
    bool m_ok = false;
};

class SGFlatColorMaterial : public SGMaterial
{
public:
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

    virtual const E_MaterialType type() const override
    {
        return E_MaterialType::FlatColorMaterialType;
    }

    virtual bool build() override;

//protected:
    GLuint m_vbo;
    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;
};

class SGShaderManager
{
public:
    static SGMaterial* find(E_MaterialType type);
private:
    static std::map<E_MaterialType, SGMaterial*> g_materials;
};

#endif // SGMATERIAL_H
