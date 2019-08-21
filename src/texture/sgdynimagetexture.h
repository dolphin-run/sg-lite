#ifndef SGDYNIMAGETEXTURE_H
#define SGDYNIMAGETEXTURE_H
#include "sgtexture.h"


class SGDynImageTexture : public SGTexture
{
public:
    SGDynImageTexture();
    ~SGDynImageTexture();

    virtual void build() override;
    virtual void bind() override;
    virtual Size size() const override;
    virtual unsigned add(const Image &img) override; 
    virtual bool update(unsigned key, const Image &img) override;

protected:

protected:
    unsigned m_texture = { 0 };
};

#endif // SGDYNIMAGETEXTURE_H
