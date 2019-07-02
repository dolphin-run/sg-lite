#ifndef SGMATERIAL_H
#define SGMATERIAL_H

#include <QOpenGLShaderProgram>
#include "sgstruct.h"

//define shader used to render
enum E_MaterialType{
    FlatColorMaterialType,
    TextureMaterialType
};

class SGMaterialShader
{
public:
    SGMaterialShader() {}
    virtual ~SGMaterialShader() {}
    virtual void updateState(const QMatrix4x4 &projection) {}
    virtual char const* const* attributeNames() const = 0;
    inline QOpenGLShaderProgram *program() { return &m_program; }
protected:
    virtual void compile();
    virtual const char *vertexShader() const = 0;
    virtual const char *fragmentShader() const = 0;
protected:
    QOpenGLShaderProgram m_program;
};

class SGMaterial
{
public:
    class SGMaterialType {};
    virtual ~SGMaterial() {}
    virtual E_MaterialType type() const = 0; 
    virtual SGMaterialShader *createShader() const = 0;
};

#endif // SGMATERIAL_H
