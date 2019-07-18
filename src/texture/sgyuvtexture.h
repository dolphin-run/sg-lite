#ifndef SGYUVTEXTURE_H
#define SGYUVTEXTURE_H
#include "sgtexture.h"

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

class SGYuvTexture : public SGTexture
{
public:
    //cell limit should be removed in the future.
    SGYuvTexture();
    ~SGYuvTexture();

    void setYuvInfo(SYUVInfo *info);

    virtual void build() override;
    virtual void bind() override;
    virtual Size size() const override;
    virtual unsigned add(const Image &img) override;

protected:

protected:
    //TexBuffer m_imgBuf;

    int gl_fmt;
    unsigned m_textureY = 0;
    unsigned m_textureU = 0;
    unsigned m_textureV = 0;
    SYUVInfo *m_yuvInfo = nullptr;
};

#endif // SGYUVTEXTURE_H
