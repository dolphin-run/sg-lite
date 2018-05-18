#ifndef SGGLYPHMATERIAL_H
#define SGGLYPHMATERIAL_H

#include "sgmaterial.h"
#include "util/sgglyph.h"


class SGTexture;
class SGGlyphMaterial : public SGMaterial
{
public:
    SGGlyphMaterial();
    virtual E_MaterialType type() const override;
    virtual SGMaterialShader *createShader() const override;

    const GlyphInfo& setGlyph(wchar_t ch);
    SGTexture *texture() const;

private:
    SGTexture *m_tex = nullptr;
};

#endif // SGGLYPHMATERIAL_H
