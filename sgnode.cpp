#include "sgnode.h"

bool SGMaterial::compile()
{
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();
    return true;
}

bool SGFlatColorMaterial::build()
{
    bool stat = compile();
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    return stat;
}

SGNode::SGNode(SGNode* parent)
{
    if(parent) parent->add(this);
}

void SGNode::add(SGNode* node)
{
    node->m_parent = this;
    m_children.push_back(node);
}

Rect SGNode::boundingRect() const
{
    return Rect();
}

SGRectangleNode::SGRectangleNode(SGNode * parent):
    SGNode(parent)
{
    //init material
    m_material = new SGFlatColorMaterial();
}

void SGRectangleNode::setColor(int32_t c)
{
//    color.a = c&0xff;
//    c>>=8;
    color.b = c&0xff;
    c>>=8;
    color.g = c&0xff;
    c>>=8;
    color.r = c&0xff;
}
