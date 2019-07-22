#ifndef SGRENDERER_H
#define SGRENDERER_H

#include "sgnode.h"
#include <list>
#include <vector>
#include <unordered_map>

class SGView;

//just for rendering
struct Node {
    const SGNode *gnode = nullptr;

    Node *prev = nullptr;
    Node *next = nullptr;

    void updateMatrix();

    glm::mat4 matrix;
    unsigned batch : 1;
    unsigned visible : 1;

    //absolute properties according to the scene.
    float x;
    float y;
    float z;
    float width;
    float height;
    float opacity;
    float rotation;

    RectF bounding;

    unsigned dirty;
};

struct Buffer {
    unsigned id = 0;

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

template<typename Type>
class AllocPool 
{
public:
    Type* alloc() {
        if (pool.size())
        {
            auto it = pool.back();
            pool.pop_back();
            return it;
        }
        else
        {
            return new Type;
        }
    }
    void free(Type *p) {
        pool.push_back(p);
    }

private:
    std::vector<Type*> pool;
};

class SGShaderManager
{
public:
    SGMaterialShader* find(SGMaterial *material);
private:
    std::unordered_map<E_MaterialType, SGMaterialShader*> m_shaders;
};

class SGRenderer
{
    enum E_RebuildFlag {
        BuildRenderLists = 0x0001,
        BuildBatches = 0x0002,
        FullRebuild = 0xffff
    };
public:
    SGRenderer(SGView *view);
    virtual bool initialize();

    void setRootNode(const SGRootNode *root);
    void clear();
    virtual void render();

    //
    virtual void itemAdded(SGNode *node);
    virtual void itemDeleted(SGNode *node);
    virtual void itemChanged(SGNode *node, SGNode::E_DirtyType changes);

protected:
    void buildRenderNodes();
    void syncGnode(const SGNode *node, Node *p);
    void selectRenderNodes(const SGNode *node);
    void buildOpacityBatches();
    void buildAlphaBatches();

    void map(Buffer *buffer);
    void unmap(Buffer *buffer);

    void uploadBatches(Batch *batch);
    void renderOpacityBatches();
    void renderAlphaBatches();
    void renderBatch(Batch *batch);

    bool checkOverlap(int first, int last, const RectF &bounds);

protected:
    AllocPool<Batch> m_batchPool;
    AllocPool<Node> m_nodePool;
    std::vector<char> m_mapMemoryCache;
    std::vector<Batch *> m_opaqueBatches;
    std::vector<Batch *> m_alphaBatches;
    std::vector<Node*> m_opacityNodes;
    std::vector<Node*> m_alphaNodes;
    std::unordered_map<const SGNode*, Node*> m_mapNodes;
    const SGRootNode *m_rootNode;

    SGShaderManager m_shrMgr;

    SGView *m_view = nullptr;

    int m_zRenderOrder;
    unsigned m_rebuild;
};

#endif // SGRENDERER_H
