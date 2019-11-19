#ifndef SGGRIDIMAGETEXTURE_H
#define SGGRIDIMAGETEXTURE_H
#include "sgtexture.h"

/*
** image must be format as rgba, like 'PNG'
** image size(px) level:
** 64*64
** 128*128
** 256*256
** 512*512
*/

struct ImageGridBuffer : TexBuffer
{
    int cellWid = 256;
    int cellHei = 256;

    int sentinal = 0;//next empty cell.
    int maxRow = 0;
    int maxCol = 0;

    int gl_fmt;
};

class SGGridImageTexture : public SGTexture
{
public:
    //cell limit should be removed in the future.
    SGGridImageTexture(int pxwid, int pxhei, Image::ImageFormat pxfmt, int cellwid, int cellhei);
    ~SGGridImageTexture();

    virtual void build() override;
    virtual void bind() override;
    virtual Size size() const override;
    virtual unsigned add(const Image &img) override; 
    virtual bool update(unsigned key, const Image &img) override;

protected:

protected:
    ImageGridBuffer m_imgBuf;
    unsigned m_id = 0;
    bool m_changed = false;
};

#endif // SGGRIDIMAGETEXTURE_H
