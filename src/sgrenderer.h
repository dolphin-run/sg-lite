#ifndef SGRENDERER_H
#define SGRENDERER_H

#include <QOpenGLFunctions>
#include "sgnode.h"
#include <list>
#include <unordered_map>

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
    bool dirty = true;

    Buffer vbo;

    unsigned int degenerate : 1;
};

class BatchPool 
{
public:
    Batch* alloc() {
        if (pool.size())
        {
            auto it = pool.back();
            pool.pop_back();
            return it;
        }
        else
        {
            return new Batch;
        }
    }
    void free(Batch *p) {
        pool.push_back(p);
    }

private:
    std::vector<Batch*> pool;
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

    void sync(const SGRootNode *root);
    void syncGnode(const SGNode *node);
    virtual void render();

protected:
    void selectRenderNodes();
    void buildOpacityBatches();

    void map(Buffer *buffer);
    void unmap(Buffer *buffer);

    void uploadBatches();
    void renderBatches();

protected:
    BatchPool m_batchPool;
    std::vector<char> m_mapMemoryCache;
    std::list<Batch *> m_opaqueBatches;
    std::list<Batch *> m_alphaBatches;
    std::vector<Node*> m_opacityNodes;
    std::vector<Node*> m_alphaNodes;
    std::unordered_map<const SGNode*, Node*> m_mapNodes;

    SGShaderManager m_shrMgr;

    SGView *m_view = nullptr;
};

#endif // SGRENDERER_H
