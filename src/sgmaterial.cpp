#include "sgmaterial.h"

void SGMaterialShader::compile()
{
    m_program.build(vertexShader(), fragmentShader());
}
