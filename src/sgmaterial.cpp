#include "sgmaterial.h"

SGMaterialShader::SGMaterialShader()
{}

SGMaterialShader::~SGMaterialShader()
{}

void SGMaterialShader::compile()
{
    m_program.build(vertexShader(), fragmentShader(), attributeNames());
}
