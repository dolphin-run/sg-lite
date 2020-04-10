#ifndef SGRENDERER_H
#define SGRENDERER_H

#include "sgnode_p.h"
#include "sgrootnode_p.h"
#include <list>
#include <vector>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "sgmaterial.h"

class SGView;
//class SGMaterial;
//class SGMaterialShader;

//just for rendering
struct Node {
    const SGNode *gnode = nullptr;

    Node *prev = nullptr;
    Node *next = nullptr;

    void updateMatrix(const Node *pn);

    glm::vec3 center;
    glm::mat4 matrix;
    glm::mat4 matrixScale;
    glm::mat4 matrixRotate;
    unsigned alpha : 1;
    unsigned batch : 1;
    unsigned visible : 1;
    unsigned geometryChanged : 1;
    unsigned opacityChanged : 1;
    unsigned visibleChanged : 1;

    //absolute properties according to the scene.
    float x;
    float y;
    float z;
    float width;
    float height;
    float opacity;
    float rotation;
    float scale;

    RectF bounding;
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
    std::unordered_map<E_MaterialType, SGMaterialShader*, std::hash<int>> m_shaders;
};

class SGRenderer
{
    enum E_RebuildFlag {
        BuildNodes = 0x0001,
        SelectNodes = 0x0002,
        BuildBatches = 0x0004,
        UploadBatches = 0x0008,
        FullRebuild = 0xffff
    };
public:
    SGRenderer(SGView *view);
    virtual bool initialize();

    void setRootNode(const SGRootNode *root);
    void clear();
    virtual void render();

    //
    virtual void itemAdded(SGNode *gnode);
    virtual void itemDeleted(SGNode *gnode);
    virtual void itemChanged(SGNode *gnode, SGNode::E_DirtyType changes);

protected:
    void buildRenderNodes(const SGNode *gnode, Node *p);
    void selectRenderNodes();
    void buildOpacityBatches();
    void buildAlphaBatches();

    void map(Buffer *buffer);
    void unmap(Buffer *buffer);

    void uploadBatches();
    void uploadOneBatch(Batch *batch);
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
    Node m_rootNode;
    const SGRootNode *m_rootGNode;

    SGShaderManager m_shrMgr;

    SGView *m_view = nullptr;

    int m_zRenderOrder;
    unsigned m_rebuild = FullRebuild;
};

#endif // SGRENDERER_H
