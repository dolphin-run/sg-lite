#ifndef SGCOLORMATERIAL_H
#define SGCOLORMATERIAL_H

#include "sgmaterial.h"

class SGFlatColorMaterial : public SGMaterial
{
public:
    virtual E_MaterialType type() const override;
    virtual SGMaterialShader *createShader() const override;
};

#endif // SGCOLORMATERIAL_H
