#ifndef SGGLYPHTEXTURE_H
#define SGGLYPHTEXTURE_H
#include "sgtexture.h"

/*
** image must be format as rgba, like 'PNG'
** image size(px) level:
** 64*64
** 128*128
** 256*256
** 512*512
*/

struct TexGlyphBuffer : TexBuffer
{
    int cellWid = 64;
    int cellHei = 64;

    int sentinal = 0;//next empty cell.
    int maxRow = 0;
    int maxCol = 0;

    virtual void init(int wid, int hei, int comp) override;
};

class SGGlyphTexture : public SGTexture
{
public:
    SGGlyphTexture();
    ~SGGlyphTexture();

    virtual void build() override;
    virtual void bind() override;
    virtual int textureId() const override;
    virtual Size size() const override;
    virtual unsigned add(const char* filename) override;
    unsigned add(const Image &img);

protected:

protected:
    TexGlyphBuffer m_glyphBuf;
    unsigned m_id = 0;
};

#endif // SGGLYPHTEXTURE_H
