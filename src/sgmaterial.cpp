#include "sgmaterial.h"


void SGMaterialShader::compile()
{
    m_program.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShader());
    m_program.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShader());
    if(!m_program.link())
        qDebug() << "SGMaterialShader Compile Error." << m_program.log();
}
