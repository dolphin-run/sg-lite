#ifndef SGMATERIAL_H
#define SGMATERIAL_H

#include "util/glshaderprogram.h"
#include "sgstruct.h"

//define shader used to render
enum E_MaterialType{
    FlatColorMaterialType,
    TextureMaterialType,
    GlyphMaterialType,
    YuvMaterialType,
    DynImageMaterialType
};

class SGMaterialShader
{
public:
    SGMaterialShader();
    virtual ~SGMaterialShader();
    virtual void updateState(const float *mat4, const SGNodeState *stat) = 0;
    virtual char const* const* attributeNames() const = 0;
    inline GLShaderProgram *program() { return &m_program; }
protected:
    virtual void compile();
    virtual const char *vertexShader() const = 0;
    virtual const char *fragmentShader() const = 0;
protected:
    GLShaderProgram m_program;
};

class SGMaterial
{
public:
    class SGMaterialType {};
    virtual ~SGMaterial() {}
    virtual E_MaterialType type() const = 0; 
    virtual SGMaterialShader *createShader() const = 0;
    virtual bool share() const { return true; }

    //will be called from rendering thread 
    virtual void syncState() {}
};

#endif // SGMATERIAL_H
