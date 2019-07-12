#include "sgmaterial.h"

QMatrix4x4 glmtoqmat(const glm::mat4 &mat)
{
    QMatrix4x4 qmat;
    memcpy(qmat.data(), &mat[0][0], sizeof(mat));
    return qmat;
};

void SGMaterialShader::compile()
{
    m_program.setShader(vertexShader(), fragmentShader());
    //if(!m_program.link())
    //    qDebug() << "SGMaterialShader Compile Error." << m_program.log();
}
