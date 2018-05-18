#ifndef SGYUVTEXTURE_H
#define SGYUVTEXTURE_H
#include "sgtexture.h"


class SGYuvTexture : public SGTexture
{
public:
    enum E_YUVComponent
    {
        Y_Component,
        U_Component,
        V_Component
    };
    //cell limit should be removed in the future.
    SGYuvTexture();
    ~SGYuvTexture();

    virtual void build() override;
    virtual void bind() override;
    virtual Size size() const override;
    virtual unsigned add(const Image &img) override; 
    virtual bool update(unsigned key, const Image &img) override;

protected:

protected:
    //TexBuffer m_imgBuf;

    int gl_fmt;
    unsigned m_texture[3] = { 0,0,0 };
};

#endif // SGYUVTEXTURE_H
