#ifndef SGTEXTUREMATERIAL_H
#define SGTEXTUREMATERIAL_H

#include "../sgmaterial.h"
#include "../util/sgimagetexture.h"

class SGTextureMaterial : public SGMaterial
{
public:
    SGTextureMaterial();
    virtual E_MaterialType type() const override;
    virtual SGMaterialShader *createShader() const override;

    const RectF& setImage(const char *path);
    SGTexture *texture() const;

private:
    SGTexture *m_tex = nullptr;
};

#endif // SGTEXTUREMATERIAL_H
