#ifndef SGRENDERER_H
#define SGRENDERER_H

#include <QOpenGLFunctions>
#include "sgnode.h"
#include <list>

struct Node {
    SGNode *gnode = nullptr;

    Node *prev = nullptr;
    Node *next = nullptr;
};

struct Buffer {
    GLuint id = 0;

    size_t verticeNum = 0;
    size_t size = 0;
    void *data = nullptr;
};

struct Batch {
    Node *firstChild = nullptr;

    Buffer vbo;
};

class SGShaderManager
{
public:
    SGMaterialShader* find(SGMaterial *material);
private:
    std::map<E_MaterialType, SGMaterialShader*> g_ms;
};

class SGRenderer : public QOpenGLFunctions
{
public:
    SGRenderer();
    virtual void initializeGL();

    virtual void render(int width, int height);

protected:
    void buildOpacityBatches();

    void map(Buffer *buffer);
    void unmap(Buffer *buffer);

    void uploadBatches();
    void renderBatches();

protected:
    void test_Alloc();
    std::list<Batch *> m_opaqueBatches;
    std::list<Batch *> m_alphaBatches;
    std::list<Node*> m_opacity_nodes;
    std::list<Node*> m_alpha_nodes;
    QMatrix4x4 m_projection;

    SGShaderManager m_shrMgr;
};

#endif // SGRENDERER_H
