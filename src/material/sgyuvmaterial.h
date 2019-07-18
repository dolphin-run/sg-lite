#ifndef SGYUVMATERIAL_H
#define SGYUVMATERIAL_H

#include "sgmaterial.h"
#include "texture/sgyuvtexture.h"

class SGTexture;
class SGYuvMaterial : public SGMaterial
{
public:
    SGYuvMaterial();
    virtual E_MaterialType type() const override;
    virtual SGMaterialShader *createShader() const override;

    void setYuv(const unsigned char *src);
    SGTexture *texture() const;

private:
    mutable SGYuvTexture m_tex;
};

#endif // SGYUVMATERIAL_H
