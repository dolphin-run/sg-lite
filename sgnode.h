#ifndef SGNODE_H
#define SGNODE_H
#include <list>

#include <QOpenGLShaderProgram>
#include "sgstruct.h"
#include "sggeometry.h"
#include "sgmaterial.h"


class SGNode
{
public:
    enum E_NodeType {BasicNodeType, RectangleNodeType};
    SGNode(SGNode* parent=nullptr);
    virtual ~SGNode();
    void add(SGNode* node);
    Rect boundingRect() const;

    //properties
    int x=0;//left
    int y=0;//top
    int z=0;//z-index
    float opacity=1.f;
    int width=0;//in scene's pixel size
    int height=0;

    void renderProcessor();
    void updateMatrix();

    int gx = 0;//global coordinate to the scene
    int gy = 0;
    int gwid = 0;
    int ghei = 0;
    int gopacity=1.f;


    QMatrix4x4 matrix;

//protected:
    //
    bool m_alpha=false;
    SGNode* m_parent=nullptr;
    std::list<SGNode*> m_children;

    SGGeometry *m_geometry=nullptr;
    SGMaterial *m_material = nullptr;
};

class SGRectangleNode : public SGNode
{
public:
    SGRectangleNode(SGNode* parent = nullptr);
    void setColor(int32_t c);

private:
    Color m_color;
    SGColorRectangleGeometry m_colorGeometry;
};

class SGImageNode : public SGNode
{};

class SGTextNode : public SGNode
{};


class SGLineNode : public SGRectangleNode
{};

class SGUserDefined : public SGNode{};
#endif // SGNODE_H
