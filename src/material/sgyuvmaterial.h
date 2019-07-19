#ifndef SGYUVMATERIAL_H
#define SGYUVMATERIAL_H

#include "sgmaterial.h"
#include "texture/sgyuvtexture.h"


const int VIDEO_PIXEL_WIDTH = 1920;
const int VIDEO_PIXEL_HEIGHT = 1080;
struct SYUVInfo
{
    SYUVInfo(int pixWid = VIDEO_PIXEL_WIDTH, int pixHei = VIDEO_PIXEL_HEIGHT);
    bool update(int pixWid, int pixHei);
    bool setViewSize(int pixWid, int pixHei);

    unsigned m_wid;
    float m_semiWid;
    unsigned m_hei;
    float m_semiHei;
    unsigned m_len;

    unsigned m_yWid;
    unsigned m_yHei;
    unsigned m_uWid;
    unsigned m_uHei;

    unsigned m_yLen;
    unsigned m_uLen;
    float m_yuvRadio;

    int m_viewWid;
    int m_viewHei;
    float m_renderScale;
};

class SGTexture;
class SGYuvMaterial : public SGMaterial
{
public:
    SGYuvMaterial();
    virtual E_MaterialType type() const override;
    virtual SGMaterialShader *createShader() const override;

    void setYuvInfo(SYUVInfo *info);

    void setYuv(const unsigned char *src);
    SGTexture *texture() const;

private:
    mutable SGYuvTexture m_tex;

    SYUVInfo *m_yuvInfo = nullptr;
};

#endif // SGYUVMATERIAL_H
