#ifndef SGIMAGETEXTURE_H
#define SGIMAGETEXTURE_H
#include "sgtexture.h"

/*
** image must be format as rgba, like 'PNG'
** image size(px) level:
** 64*64
** 128*128
** 256*256
** 512*512
*/

struct TexImageBuffer : TexBuffer
{
    int cellWid = 256;
    int cellHei = 256;

    int sentinal = 0;//next empty cell.
    int maxRow = 0;
    int maxCol = 0;

    virtual void init(int wid, int hei, int comp) override;
};

class SGImageTexture : public SGTexture
{
public:
    SGImageTexture();
    ~SGImageTexture();

    virtual void build() override;
    virtual void bind() override;
    virtual int textureId() const override;
    virtual Size size() const override;
    virtual unsigned add(const char* filename) override;

protected:

protected:
    TexImageBuffer m_imgBuf;
    unsigned m_id = 0;
};

#endif // SGIMAGETEXTURE_H
