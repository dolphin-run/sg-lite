#include "sgmaterial.h"

SGMaterial::~SGMaterial()
{
    if(m_program)
        delete m_program;
}

void SGMaterial::active()
{
    if (!m_ok)
    {
        m_ok = build();
    }
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

    return stat;
}


std::map<E_MaterialType, SGMaterial*> SGShaderManager::g_materials;

SGMaterial * SGShaderManager::find(E_MaterialType type)
{
    if (g_materials.count(type)) return g_materials[type];

    SGMaterial *material = nullptr;
    switch (type)
    {
    case FlatColorMaterialType:
        material = new SGFlatColorMaterial();
        break;
    default:
        break;
    }
    g_materials.insert(std::make_pair(type, material));
    return material;
}
