#include "sgrenderer.h"
#include "node/sgcircularnode.h"
#include "node/sgrectanglenode.h"
#include "sgview.h"


void Node::renderProcessor()
{
    //rx = 
}

void Node::updateMatrix()
{
    if (gnode->m_dirty)
    {
        matrix.setToIdentity();
        matrix.translate(x + width / 2, y + height / 2);
        matrix.scale(width, height);

        gnode->m_dirty = 0;
    }
}

SGMaterialShader * SGShaderManager::find(SGMaterial *material)
{
    E_MaterialType type = material->type();
    if (g_ms.count(type)) return g_ms[type];

    SGMaterialShader *shader = material->createShader();

    QOpenGLShaderProgram *p = shader->program();
    char const *const *attr = shader->attributeNames();
    for (int i = 0; attr[i]; ++i) {
        if (*attr[i])
            p->bindAttributeLocation(attr[i], i);
    }
    
    g_ms.insert(std::make_pair(type, shader));
    return shader;
}

SGRenderer::SGRenderer(SGView *view) :
    m_view(view)
{
    m_mapMemoryCache.reserve(1024);
}

void SGRenderer::initializeGL()
{
    initializeOpenGLFunctions();
}

void SGRenderer::sync(const SGRootNode *root)
{
    m_rootNode = root;
    m_zRenderOrder = 1;
    Node *c = new Node; c->gnode = root;
    c->opacity = 1.f;
    c->x = 0;
    c->y = 0;

    auto children = root->m_children;
    children.sort([](const SGNode *lop, const SGNode *rop) {
        return lop->z() < rop->z();
    });

    for (auto itm : children)
        syncGnode(itm, c);
}

void SGRenderer::syncGnode(const SGNode * node, Node *p)
{
    if (!node->m_visible) return;

    Node *c = new Node; c->gnode = node;
    c->opacity = p->opacity*node->m_opacity;
    c->x = p->x + node->m_x;
    c->y = p->y + node->m_y;
    c->z = m_zRenderOrder++ / 1000.f;
    c->width = node->m_width;
    c->height = node->m_height;
    c->bounding = RectF(c->x, c->y, c->width, c->height);

    m_mapNodes.insert(std::make_pair(node, c));

    auto children = node->m_children;
    children.sort([](const SGNode *lop, const SGNode *rop) {
        return lop->z() < rop->z();
    });
    for (auto child : children)
    {
        syncGnode(child, c);
    }
}

void SGRenderer::render()
{
    m_opacityNodes.clear();
    m_alphaNodes.clear(); 
    selectRenderNodes(m_rootNode);

    buildOpacityBatches();
    buildAlphaBatches();

    for (Batch *batch : m_opaqueBatches)
    {
        if (!batch->dirty) continue;
        batch->dirty = false;
        uploadBatches(batch);
    }
    for (Batch *batch : m_alphaBatches)
    {
        if (!batch->dirty) continue;
        batch->dirty = false;
        uploadBatches(batch);
    }

    const qreal retinaScale = m_view->devicePixelRatio();
    glViewport(0, 0, m_view->width() * retinaScale, m_view->height() * retinaScale);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //render
    renderOpacityBatches();
    renderAlphaBatches();
}

void SGRenderer::selectRenderNodes(const SGNode *node)
{
    if (node->type() == SGNode::E_NodeType::ImageNodeType || node->type() == SGNode::E_NodeType::TextNodeType)
    {
        m_mapNodes[node]->updateMatrix();
        m_mapNodes[node]->batch = 0;
        m_alphaNodes.push_back(m_mapNodes[node]);
    }
    else if(node->type() == SGNode::E_NodeType::RectangleNodeType || node->type() == SGNode::E_NodeType::CircularNodeType)
    {
        m_mapNodes[node]->updateMatrix();
        m_mapNodes[node]->batch = 0;
        m_opacityNodes.push_back(m_mapNodes[node]);
    }
    for (auto itm : node->m_children)
    {
        selectRenderNodes(itm);
    }
}

void SGRenderer::buildOpacityBatches()
{
    for (auto itm : m_opaqueBatches)
    {
        m_batchPool.free(itm);
    }
    m_opaqueBatches.clear();

    for (int i = 0; i < m_opacityNodes.size(); i++)
    {
        Node *node = m_opacityNodes[i];

        if (node->batch) continue;

        Batch *batch = m_batchPool.alloc();
        batch->firstChild = node;
        node->prev = nullptr;
        node->batch = 1;

        SGNode::E_NodeType ntype = node->gnode->type();
        batch->degenerate = node->gnode->m_geometry->renderType() == E_RenderType::E_RenderTriangleStrip;
        int nodenum = 1;
        batch->vbo.verticeNum = node->gnode->m_geometry->verticeNum();
        for (int j = i + 1; j < m_opacityNodes.size(); j++)
        {
            Node *nj = m_opacityNodes[j];

            if (nj->batch ||
                ntype != nj->gnode->type())
                continue;

            node->next = nj;
            nj->prev = node;
            nj->batch = 1;
            batch->vbo.verticeNum += nj->gnode->m_geometry->verticeNum();

            node = nj;
            nodenum++;
        }
        node->next = nullptr;

        if (batch->degenerate)
        {
            batch->vbo.verticeNum += (nodenum - 1) * 2;
        }
        batch->vbo.size = batch->vbo.verticeNum * node->gnode->m_geometry->verticeSize();
        
        batch->dirty = true;
        m_opaqueBatches.push_back(batch);
    }
}

bool SGRenderer::checkOverlap(int first, int last, const RectF &bounds)
{
    for (int i = first; i <= last; i++) {
        Node *n = m_alphaNodes.at(i);
        if (n->batch)
            continue;
        if (n->bounding.intersects(bounds))
            return true;
    }
    return false;
}

void SGRenderer::buildAlphaBatches()
{
    for (auto itm : m_alphaBatches)
    {
        m_batchPool.free(itm);
    }
    m_alphaBatches.clear();

    std::sort(m_alphaNodes.begin(), m_alphaNodes.end(), [](const Node *lop, const Node *rop) ->bool {return lop->z > rop->z; });
    for (int i = 0; i < m_alphaNodes.size(); i++)
    {
        Node *node = m_alphaNodes[i];

        if (node->batch) continue;

        Batch *batch = m_batchPool.alloc();
        batch->firstChild = node;
        node->prev = nullptr;
        node->batch = 1;

        RectF overlapBounds;
        overlapBounds.set(FLT_MAX, FLT_MAX, FLT_MIN, FLT_MIN);

        SGNode::E_NodeType ntype = node->gnode->type();
        batch->degenerate = node->gnode->m_geometry->renderType() == E_RenderType::E_RenderTriangleStrip;
        int nodenum = 1;
        batch->vbo.verticeNum = node->gnode->m_geometry->verticeNum();
        for (int j = i + 1; j < m_alphaNodes.size(); j++)
        {
            Node *nj = m_alphaNodes[j];

            if (nj->batch)
                continue;

            if (ntype == nj->gnode->type())
            {
                if (overlapBounds.intersects(nj->bounding) && checkOverlap(i + 1, j - 1, nj->bounding))//not overlap
                {
                    break;
                }

                node->next = nj;
                nj->prev = node;
                nj->batch = 1;
                batch->vbo.verticeNum += nj->gnode->m_geometry->verticeNum();

                node = nj;
                nodenum++;
            }
            else
            {
                overlapBounds |= nj->bounding;
            }
        }
        node->next = nullptr;

        if (batch->degenerate)
        {
            batch->vbo.verticeNum += (nodenum - 1) * 2;
        }
        batch->vbo.size = batch->vbo.verticeNum * node->gnode->m_geometry->verticeSize();

        batch->dirty = true;
        m_alphaBatches.push_back(batch);
    }
}

void SGRenderer::map(Buffer *buffer)
{
    if (buffer->size > m_mapMemoryCache.capacity())
    {
        m_mapMemoryCache.reserve((buffer->size + 1023) / 1024 * 1024);
    }
    
    buffer->data = m_mapMemoryCache.data();
}

void SGRenderer::unmap(Buffer *buffer)
{
    if (!buffer->id)
    {
        glGenBuffers(1, &buffer->id);
    }
    glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
    glBufferData(GL_ARRAY_BUFFER, buffer->size, buffer->data, GL_STREAM_DRAW);
}

void SGRenderer::uploadBatches(Batch *batch)
{
    map(&batch->vbo);

    //write data
    Node *node = batch->firstChild;
    char* pd = (char*)batch->vbo.data;
    while (node)
    {
        SGGeometry *gg = node->gnode->m_geometry;

        char* pdmain = pd;
        if (node->prev && batch->degenerate)
        {
            pdmain = pd + gg->verticeSize();
        }
        memcpy(pdmain, gg->data(), gg->size());

        //apply transformation
        VertexAsPointColor *pvr = (VertexAsPointColor *)pdmain;
        for (int i = 0; i < gg->verticeNum(); i++)
        {
            PointF *pt = (PointF *)(pvr + i);
            pt->map(node->matrix);
            (pvr + i)->z = node->z;
        }

        if (node->prev && batch->degenerate)
        {
            memcpy(pd, pdmain, gg->verticeSize());
            pd += gg->verticeSize();
        }
        pd += gg->size();

        node = node->next;

        if (node && batch->degenerate)
        {
            //add degenerate node
            memcpy(pd, pd - gg->verticeSize(), gg->verticeSize());
            pd += gg->verticeSize();
        }
    }

    //end write

    unmap(&batch->vbo);
}

void SGRenderer::renderOpacityBatches()
{
    glClearDepthf(1); // calls glClearDepth() under the hood for desktop OpenGL
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(true);
    glDisable(GL_BLEND);

    glDisable(GL_CULL_FACE);
    glColorMask(true, true, true, true);
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_STENCIL_TEST);

    //
    std::sort(m_opaqueBatches.begin(), m_opaqueBatches.end(), [](Batch* left, Batch *right)->bool {return left->firstChild->z > right->firstChild->z; });
    for (Batch *batch : m_opaqueBatches)
    {
        renderBatch(batch);
    }
}

void SGRenderer::renderAlphaBatches()
{
    glEnable(GL_BLEND);
    glDepthMask(false);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//GL_SRC_ALPHA  GL_ONE

    std::sort(m_alphaBatches.begin(), m_alphaBatches.end(), [](Batch* left, Batch *right)->bool {return left->firstChild->z < right->firstChild->z; });
    for (Batch *batch : m_alphaBatches)
    {
        renderBatch(batch);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDepthMask(true);
}

void SGRenderer::renderBatch(Batch * batch)
{
    Node *n = batch->firstChild;
    Q_ASSERT(n);

    const SGNode *gn = n->gnode;

    SGMaterialShader *shader = m_shrMgr.find(gn->m_material);

    bool ret = shader->program()->bind();

    shader->updateState(m_view->transform());

    glBindBuffer(GL_ARRAY_BUFFER, batch->vbo.id);

    auto attrs = gn->m_geometry->attributes();
    for (int i = 0; i < attrs->vparanum; i++)
    {
        glVertexAttribPointer(i, attrs->vas[i].size, GL_FLOAT, GL_FALSE, attrs->vsize, (void*)(attrs->vas[i].offset));
        glEnableVertexAttribArray(i);
    }

    glDrawArrays(gn->m_geometry->renderType(), 0, batch->vbo.verticeNum);
}
