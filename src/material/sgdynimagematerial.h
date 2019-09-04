#ifndef SGDYNIMAGEMATERIAL_H
#define SGDYNIMAGEMATERIAL_H

#include "sgmaterial.h"
#include "texture/sgdynimagetexture.h"
#include <mutex>

class SGTexture;
class SGDynImageMaterial : public SGMaterial
{
public:
    SGDynImageMaterial();
    virtual E_MaterialType type() const override;
    virtual SGMaterialShader *createShader() const override;
    virtual bool share() const { return false; }

    void setSize(int wid, int hei);
    void setRgb(const unsigned char *src);
    SGTexture *texture() const;

    virtual void syncState() override;

private:
    mutable SGDynImageTexture m_tex;
    unsigned m_yuvChanged : 1;
    unsigned char * m_yuvBuf = nullptr;
    unsigned m_yuvBufSize = 0;
    std::mutex m_yuvLock;

    int m_width;
    int m_height;
};

#endif // SGDYNIMAGEMATERIAL_H
