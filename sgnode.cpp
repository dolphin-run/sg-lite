#include "sgnode.h"
#include <QOpenGLFunctions>


std::vector<SGMaterial*> g_Materials;

SGMaterial::~SGMaterial()
{
    if(m_program)
        delete m_program;
}

bool SGMaterial::compile()
{
    m_program = new QOpenGLShaderProgram;
    bool ret = m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShader());
    ret = m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShader());
    ret = m_program->link();
    qDebug() << m_program->log();
    return true;
}

bool SGFlatColorMaterial::build()
{
    bool stat = compile();
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");
    
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    glFuncs.glGenBuffers(1, &m_vbo);
    glFuncs.glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glFuncs.glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glFuncs.glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
    glFuncs.glEnableVertexAttribArray(0);
    glFuncs.glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));
    glFuncs.glEnableVertexAttribArray(1);

    glFuncs.glBindBuffer(GL_ARRAY_BUFFER, 0);
    return stat;
}

SGMaterial *SGNode::m_material=nullptr;

SGNode::SGNode(SGNode* parent)
{
    if(parent) parent->add(this);
}

SGNode::~SGNode()
{
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

void SGNode::renderProcessor()
{
    //rx = 
}

SGRectangleNode::SGRectangleNode(SGNode * parent):
    SGNode(parent)
{
    //init material
    if(!m_material)
    {
        m_material = new SGFlatColorMaterial();
        g_Materials.push_back(m_material);
    }
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
