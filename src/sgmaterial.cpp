#include "sgmaterial.h"

QMatrix4x4 glmtoqmat(const glm::mat4 &mat)
{
    QMatrix4x4 qmat;
    memcpy(qmat.data(), &mat[0][0], sizeof(mat));
    return qmat;
};

void SGMaterialShader::compile()
{
    m_program.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShader());
    m_program.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShader());
    if(!m_program.link())
        qDebug() << "SGMaterialShader Compile Error." << m_program.log();
}
