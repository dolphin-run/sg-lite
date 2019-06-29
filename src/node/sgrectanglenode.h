#ifndef SGRECTANGLENODE_H
#define SGRECTANGLENODE_H

#include "sgnode.h"

class SGTextureMaterial;
class SGColorRectangleGeometry;
class SGTextureRectangleGeometry;
class SGRectangleNode : public SGNode
{
public:
    SGRectangleNode(SGNode* parent = nullptr);
    void setColor(int32_t c);

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

#endif // SGRECTANGLENODE_H
