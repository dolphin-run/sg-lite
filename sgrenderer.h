#ifndef SGRENDERER_H
#define SGRENDERER_H

#include <QOpenGLFunctions>
#include "sgnode.h"
#include <list>

struct Node {
    SGNode *node = nullptr;
    Node *sibling = nullptr;
};

struct Buffer {
    GLuint id = 0;
    size_t size = 0;
    void *data = nullptr;
};

struct Batch {
    Node *firstChild = nullptr;

    Buffer vbo;
};

class SGRenderer : public QOpenGLFunctions
{
public:
    SGRenderer();

    virtual void render();

protected:
    void buildOpacityBatches();

    void map(Buffer *buffer);
    void unmap(Buffer *buffer);

    void renderBatches();

protected:
    void test_Alloc();
    std::list<Batch *> m_opaqueBatches;
    std::list<Batch *> m_alphaBatches;
    std::list<Node*> m_opacity_nodes;
    std::list<Node*> m_alpha_nodes;
};

#endif // SGRENDERER_H
