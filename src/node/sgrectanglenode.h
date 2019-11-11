#ifndef SGRECTANGLENODE_H
#define SGRECTANGLENODE_H

#include "sgnode.h"

class SGTextureMaterial;
class SGYuvMaterial;
class SGDynImageMaterial;
struct SYUVInfo;
class SGColorRectangleGeometry;
class SGTextureRectangleGeometry;
class SG_DECL_EXPORT SGRectangleNode : public SGNode
{
    SG_DISABLE_COPY(SGRectangleNode);
public:
    SGRectangleNode(SGNode* parent = nullptr);
    void setColor(Color c);

private:
    Color m_color;
    SGColorRectangleGeometry *m_colorGeometry=nullptr;
};

class SG_DECL_EXPORT SGImageNode : public SGNode
{
    SG_DISABLE_COPY(SGImageNode);
public:
    SGImageNode(SGNode* parent = nullptr);
    void setImage(const char* src);

private:
    Color m_color;
    SGTextureMaterial *m_texMaterial = nullptr;
    SGTextureRectangleGeometry *m_texGeometry=nullptr;
};

class SG_DECL_EXPORT SGYuvVideoNode : public SGNode
{
    SG_DISABLE_COPY(SGYuvVideoNode);
public:
    SGYuvVideoNode(SGNode* parent = nullptr);
    void setPixelSize(int pxWid = 1920, int pxHei = 1080);
    bool setYuv(const unsigned char* yuv = nullptr, unsigned int len = -1);//yuv set to nullptr will clear the video.

private:
    SYUVInfo *m_yuvInf = nullptr;
    SGYuvMaterial *m_texMaterial = nullptr;
    SGTextureRectangleGeometry *m_texGeometry = nullptr;
};

class SG_DECL_EXPORT SGRgbVideoNode : public SGNode
{
    SG_DISABLE_COPY(SGRgbVideoNode);
public:
    SGRgbVideoNode(SGNode* parent = nullptr);
    void setPixelSize(int pxWid = 1920, int pxHei = 1080);
    bool setRgb(const unsigned char* yuv, unsigned int len = -1);

private:
    SGDynImageMaterial *m_texMaterial = nullptr;
    SGTextureRectangleGeometry *m_texGeometry = nullptr;
};

#endif // SGRECTANGLENODE_H
