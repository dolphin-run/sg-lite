#ifndef SGTEXTUREMATERIAL_H
#define SGTEXTUREMATERIAL_H

#include "sgmaterial.h"
#include "util/sgimagetexture.h"

class SGTextureMaterial : public SGMaterial
{
public:
    SGTextureMaterial();
    virtual E_MaterialType type() const override;
    virtual SGMaterialShader *createShader() const override;

    const RectF& addImage(const char *path);
    SGTexture *texture() const;

private:
    SGTexture *m_tex = nullptr;
    std::unordered_map<std::string, unsigned> m_nameMapToKey;
    SGImageTexture m_imgTex;
};

#endif // SGTEXTUREMATERIAL_H
