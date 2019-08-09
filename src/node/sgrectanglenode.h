#ifndef SGRECTANGLENODE_H
#define SGRECTANGLENODE_H

#include "sgnode.h"

class SGTextureMaterial;
class SGYuvMaterial;
class SGColorRectangleGeometry;
class SGTextureRectangleGeometry;
class SGRectangleNode : public SGNode
{
public:
    SGRectangleNode(SGNode* parent = nullptr);
    void setColor(Color c);

private:
    Color m_color;
    SGColorRectangleGeometry *m_colorGeometry=nullptr;
};

class SGImageNode : public SGNode
{
public:
    SGImageNode(SGNode* parent = nullptr);
    void setImage(const char* src);

private:
    Color m_color;
    SGTextureMaterial *m_texMaterial = nullptr;
    SGTextureRectangleGeometry *m_texGeometry=nullptr;
};

class SGYuvVideoNode : public SGNode
{
public:
    SGYuvVideoNode(SGNode* parent = nullptr);
    void setPixelSize(int pxWid = 1920, int pxHei = 1080);
    bool setYuv(const unsigned char* yuv, int len = -1);

private:
    Size m_pxSize;
    int m_pxLen = 0;
    char *m_yuv = nullptr;
    SGYuvMaterial *m_texMaterial = nullptr;
    SGTextureRectangleGeometry *m_texGeometry = nullptr;
};

#endif // SGRECTANGLENODE_H
