#include "sgmaterial.h"

void SGMaterialShader::compile()
{
    m_program.setShader(vertexShader(), fragmentShader());
    //if(!m_program.link())
    //    qDebug() << "SGMaterialShader Compile Error." << m_program.log();
}
