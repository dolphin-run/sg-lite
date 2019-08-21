#ifndef SGDYNIMAGEMATERIAL_H
#define SGDYNIMAGEMATERIAL_H

#include "sgmaterial.h"
#include "texture/sgdynimagetexture.h"

class SGTexture;
class SGDynImageMaterial : public SGMaterial
{
public:
    SGDynImageMaterial();
    virtual E_MaterialType type() const override;
    virtual SGMaterialShader *createShader() const override;
    virtual bool share() const { return false; }

    void setRgb(const unsigned char *src);
    SGTexture *texture() const;

private:
    mutable SGDynImageTexture m_tex;
};

#endif // SGDYNIMAGEMATERIAL_H
