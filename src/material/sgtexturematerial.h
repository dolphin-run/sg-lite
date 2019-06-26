#ifndef SGTEXTUREMATERIAL_H
#define SGTEXTUREMATERIAL_H

#include "sgmaterial.h"

class SGTextureMaterial : public SGMaterial
{
public:
    virtual E_MaterialType type() const override;
    virtual SGMaterialShader *createShader() const override;
};

#endif // SGTEXTUREMATERIAL_H
