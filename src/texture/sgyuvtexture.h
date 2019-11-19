#ifndef SGYUVTEXTURE_H
#define SGYUVTEXTURE_H
#include "sgtexture.h"


class SGYuvTexture : public SGTexture
{
public:
    //cell limit should be removed in the future.
    SGYuvTexture(unsigned components);
    ~SGYuvTexture();

    virtual void build() override;
    virtual void bind() override;
    virtual Size size() const override;
    virtual unsigned add(const Image &img) override; 
    virtual bool update(unsigned compidx, const Image &img) override;

protected:

protected:
    //TexBuffer m_imgBuf;

    int gl_fmt;
    unsigned m_texture[3] = { 0,0,0 };
    unsigned m_components = 0;
};

#endif // SGYUVTEXTURE_H
