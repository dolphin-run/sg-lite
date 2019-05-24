#ifndef SGNODE_H
#define SGNODE_H
#include <list>

#include <QtGui/QOpenGLShaderProgram>

struct Color {
    int8_t r;
    int8_t g;
    int8_t b;
    //int8_t a;
};

struct Rect {
    int top;
    int left;
    int right;
    int buttom;
};

//store vertex data
//define how data should be explained
//class SGGeometry
//{
//public:
//    int8_t *data=nullptr;
//    size_t size=0;
//};

//define shader used to render
class SGMaterialType{};
class SGMaterial
{
public:
    class SGMaterialType{};
    virtual const char* fragmentShader() const = 0;
    virtual const char* vertexShader() const = 0;
    virtual const SGMaterialType *type() const = 0;
    virtual bool build() = 0;

    bool compile();
    QOpenGLShaderProgram *m_program = nullptr;
};

class SGFlatColorMaterial : public SGMaterial
{
public:
    virtual const char* fragmentShader() const override
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

    virtual const char* vertexShader() const override
    {
        return  "varying lowp vec4 col;\n"
                "void main() {\n"
                "   gl_FragColor = col;\n"
                "}\n";
    }

    virtual const SGMaterialType *type() const override
    {
        static SGMaterialType typ;
        return &typ;
    }

    virtual bool build() override;
protected:
    GLint m_posAttr;
    GLint m_colAttr;

private:
    const GLfloat vertices[] = {
        -0.5, 0.5, 0, 1,//topleft
        0.5, 0.5, 0, 1,//topright
        0.5, -0.5, 0, 1,//buttomright
        -0.5, -0.5, 0, 1//buttomleft
    };

    const GLfloat colors[] = {
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f
    };

    const GLuint verticesIndices[] = {
        0, 1, 2, 0, 2, 3
    };

    const GLint vts = 6;
};

class SGOpaqueImageMaterial : public SGMaterial
{};

class SGTranslucentImageMaterial : public SGMaterial
{};

class SGTextMaterial : public SGMaterial
{};

class SGNode
{
public:
    enum E_NodeType {BasicNodeType, RectangleNodeType};
    SGNode(SGNode* parent=nullptr);
    void add(SGNode* node);
    Rect boundingRect() const;

    //properties
    int x=0;//left
    int y=0;//top
    int z=0;//z-index
    float opacity=1.f;
    int width=0;//in scene's pixel size
    int height=0;

    int gx=0;//global coordinate to the scene
    int gy=0;
    int gopacity=1.f;

protected:
    //
    bool m_alpha=false;
    SGNode* m_parent=nullptr;
    std::list<SGNode*> m_children;

//    SGGeometry *m_geometry=nullptr;
    SGMaterial *m_material=nullptr;
};

class SGRectangleNode : public SGNode
{
public:
    SGRectangleNode(SGNode* parent = nullptr);
    void setColor(int32_t c);
protected:
    Color color;
};

class SGImageNode : public SGNode
{};

class SGTextNode : public SGNode
{};


class SGLineNode : public SGRectangleNode
{};

class SGUserDefined : public SGNode{};
#endif // SGNODE_H
