#ifndef SGSTATICIMAGETEXTURE_H
#define SGSTATICIMAGETEXTURE_H
#include "sgtexture.h"


class SGStaticImageTexture : public SGTexture
{
public:
    SGStaticImageTexture();
    ~SGStaticImageTexture();

    virtual void build() override;
    virtual void bind()override;
    virtual int textureId() const override;
    virtual Size size() const override;

protected:
    std::unordered_map<int, Rect> m_vals;
    static Rect g_invalidRect;
};

#endif // SGSTATICIMAGETEXTURE_H
