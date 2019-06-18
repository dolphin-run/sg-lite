#ifndef SGMATERIAL_H
#define SGMATERIAL_H

#include <QOpenGLShaderProgram>


struct Color {
    int8_t r;
    int8_t g;
    int8_t b;
    //int8_t a;
};

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
    virtual bool build() = 0;

    bool compile();
    QOpenGLShaderProgram *m_program = nullptr;
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

    void setColor(Color clr)
    {
        for (int i = 0; i < 4; ++i)
        {
            vertices[8 * i + 4] = clr.r / 255.f;
            vertices[8 * i + 5] = clr.g / 255.f;
            vertices[8 * i + 6] = clr.b / 255.f;
        }
    }
//protected:
    GLuint m_vbo;
    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;

//private:
    GLfloat vertices[48] = {
        0.5,  0.5, 0, 1,   1.0f, 0.0f, 0.0f, 1.0f,/*topright*/
        -0.5,  0.5, 0, 1,   1.0f, 0.0f, 0.0f, 1.0f,/*topleft*/
        0.5, -0.5, 0, 1,   1.0f, 0.0f, 0.0f, 1.0f,/*buttomright*/
        -0.5, -0.5, 0, 1,   1.0f, 0.0f, 0.0f, 1.0f/*buttomleft*/
    };
};

class SGShaderManager
{
public:
    static SGMaterial* find(E_MaterialType type);
private:
    static std::map<E_MaterialType, SGMaterial*> g_materials;
};

#endif // SGMATERIAL_H
