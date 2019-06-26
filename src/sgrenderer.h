#ifndef SGRENDERER_H
#define SGRENDERER_H

#include <QOpenGLFunctions>
#include "sgnode.h"
#include <list>
#include <map>

class QOpenGLTexture;
class SGView;
struct Node {
    const SGNode *gnode = nullptr;

    Node *prev = nullptr;
    Node *next = nullptr;

    void renderProcessor();
    void updateMatrix();

    QMatrix4x4 matrix;
    unsigned batch : 1;
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
    SGRenderer(SGView *view);
    virtual void initializeGL();

    virtual void render(const SGRootNode *root);

protected:
    void findRenderNodesFromScratch(const SGNode *root);
    void findRenderNodes(const SGNode *node);
    void buildOpacityBatches();

    void map(Buffer *buffer);
    void unmap(Buffer *buffer);

    void uploadBatches();
    void renderBatches();

protected:
    void test_Alloc();
    std::list<Batch *> m_opaqueBatches;
    std::list<Batch *> m_alphaBatches;
    std::vector<Node*> m_opacityNodes;
    std::vector<Node*> m_alphaNodes;
    std::map<SGNode*, Node*> m_nodes;
    QMatrix4x4 m_projection;

    SGShaderManager m_shrMgr;

    //
    QOpenGLTexture *m_tex;

    SGView *m_view = nullptr;
};

#endif // SGRENDERER_H
