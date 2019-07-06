#ifndef SGGLYPHMATERIAL_H
#define SGGLYPHMATERIAL_H

#include "../sgmaterial.h"

//font loader
#include "ft2build.h"
#include "freetype/freetype.h"
#include "freetype/ftglyph.h"
#include "freetype/ftoutln.h"
#include "freetype/fttrigon.h"

const int WORDLOADER_WORD_PIXEL = 60;

struct GlyphInfo
{
    wchar_t ch = 0;

    int width, height;//size
    int leftBearing, topBearing;//offset from baseline, we change the top to fit gl rendering
    int advance;//

    //only valid when loading char, we mean it...
    const unsigned char* buffer = nullptr;
    RectF tex;
};

struct LineRenderInfo
{
    std::vector<const GlyphInfo*> lineInf;
    //int m_startHPix;
    int width = 0;
};


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
