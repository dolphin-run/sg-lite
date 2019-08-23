#include "sgrenderer.h"
#include "sgview.h"
#include <algorithm>

#include <glad/glad.h>

void Node::updateMatrix()
{
    matrix = glm::mat4();
    matrix = glm::translate(matrix, glm::vec3(x + width / 2, y + height / 2, 0));
    if (abs(rotation) > glm::epsilon<float>())
    {
        matrix = glm::rotate(matrix, rotation, glm::vec3(0, 0, -1));
    }
    matrix = glm::scale(matrix, glm::vec3(width, height, 1));
}

SGMaterialShader * SGShaderManager::find(SGMaterial *material)
{
    E_MaterialType type = material->type();
    if (m_shaders.count(material)) return m_shaders.at(material);

    SGMaterialShader *shader = material->createShader();

    // Binding can be only worked before program linking.
    // GLShaderProgram *p = shader->program();
    // char const *const *attr = shader->attributeNames();
    // for (int i = 0; attr[i]; ++i) {
    //     if (*attr[i])
    //         p->bindAttributeLocation(attr[i], i);
    // }
    
    m_shaders.insert(std::make_pair(material, shader));
    return shader;
}

SGRenderer::SGRenderer(SGView *view) :
    m_view(view)
{
    m_mapMemoryCache.reserve(1024);
}

bool SGRenderer::initialize()
{
    //initialize GL;
    int ret = gladLoadGL();
    if (!ret)
    {
        printf("Failed to initialize GLAD\n");
    }
    return ret;
}

void SGRenderer::setRootNode(const SGRootNode *root)
{
    m_rootGNode = root;
    m_rootNode.gnode = root;

    assert(!m_mapNodes.count(root));
}

void SGRenderer::clear()
{
    m_mapNodes.clear();
}

void SGRenderer::buildRenderNodes(const SGNode * gnode, Node *p)
{
    assert(m_mapNodes.count(gnode));
    Node *c = m_mapNodes.at(gnode);

    c->z = m_zRenderOrder++ / 10000.f;

    if (c->geometryChanged || (p&&p->geometryChanged))
    {
        if (p)
        {
            c->x = p->x + gnode->m_x;
            c->y = p->y + gnode->m_y;
        }
        else
        {
            c->x = gnode->m_x;
            c->y = gnode->m_y;
        }
        c->width = gnode->m_width;
        c->height = gnode->m_height;
        c->rotation = gnode->m_rotation;
        c->bounding = RectF(c->x, c->y, c->width, c->height);

        c->updateMatrix();
        c->geometryChanged = 1;//set to notify children if parent geometry changed
    }

    if (c->opacityChanged || (p&&p->opacityChanged))
    {
        if (p)
        {
            c->opacity = p->opacity*gnode->m_opacity;
        }
        else
        {
            c->opacity = gnode->m_opacity;
        }

        if (c->opacity < 0.01f)
        {
            c->visible = 0;
            c->visibleChanged = 1;
        }
        c->alpha = c->opacity < 0.99f;
        c->opacityChanged = 1;//set to notify children if parent opacity changed
    }

    if (c->visibleChanged)
    {
        if (p&&p->visibleChanged)
            c->visible = gnode->m_visible ? p->visible : false;
        else
            c->visible = gnode->m_visible;
        c->visibleChanged = 1;//set to notify children if parent opacity changed
    }

    for (auto child : gnode->m_children)
    {
        buildRenderNodes(child, c);
    }

    c->geometryChanged = 0;
    c->opacityChanged = 0;
    c->visibleChanged = 0;
}

/*
** 1)walk through SGNode-Tree from root, calculate the unique `order` of each SGNode.
** 2)walk througn SGNode-Tree from root, calculate the `geometry`(transform matrix) of each SGNode.
** 3)select opacity nodes and tranlucent nodes separately.
** 4)build batches
** 5)upload batches
** 6)render batches
*/
void SGRenderer::render()
{
    if (m_rebuild&E_RebuildFlag::BuildNodes)
    {
        m_zRenderOrder = 1;
        for (auto child : m_rootGNode->m_children)
        {
            buildRenderNodes(child, nullptr);
        }
    }

    if (m_rebuild&E_RebuildFlag::SelectNodes)
    {
        m_opacityNodes.clear();
        m_alphaNodes.clear();
        selectRenderNodes();
    }

    if (m_rebuild&E_RebuildFlag::BuildBatches)
    {
        buildOpacityBatches();
        buildAlphaBatches();
    }

    if (m_rebuild&E_RebuildFlag::UploadBatches)
    {
        uploadBatches();
    }
    
    glViewport(0, 0, m_view->widthPx(), m_view->heightPx());
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //render
    renderOpacityBatches();
    renderAlphaBatches();

    m_rebuild = 0;

    //restore gl parameters for these who use sg-lite in their rendering context, for example, integrating with qt-scenegraph
    glActiveTexture(GL_TEXTURE0);
}

void SGRenderer::itemAdded(SGNode * gnode)
{
    assert(!m_mapNodes.count(gnode));
    {
        //auto pnode = gnode->m_parent;

        //parent should have been added before
        //assert(!pnode || m_mapNodes.count(pnode));

        Node *c = m_nodePool.alloc(); c->gnode = gnode;
        m_mapNodes.insert(std::make_pair(gnode, c));

        c->geometryChanged = 1;
        c->opacityChanged = 1; 
        c->visibleChanged = 1;
        m_rebuild |= E_RebuildFlag::FullRebuild;
    }
}

void SGRenderer::itemDeleted(SGNode * gnode)
{
    assert(m_mapNodes.count(gnode));
    {
        m_nodePool.free(m_mapNodes.at(gnode));
        m_mapNodes.erase(gnode);

        //can be no-rebuild
        m_rebuild |= E_RebuildFlag::BuildBatches;
        m_rebuild |= E_RebuildFlag::UploadBatches;
    }
}

void SGRenderer::itemChanged(SGNode * gnode, SGNode::E_DirtyType changes)
{
    assert(m_mapNodes.count(gnode));
    Node *node = m_mapNodes.at(gnode);
    switch (changes)
    {
    case SGNode::E_DirtyType::PositionDirty:
    case SGNode::E_DirtyType::SizeDirty:
    case SGNode::E_DirtyType::TransformDirty:
        node->geometryChanged = 1;
        m_rebuild |= E_RebuildFlag::BuildNodes;
        m_rebuild |= E_RebuildFlag::BuildBatches;
        m_rebuild |= E_RebuildFlag::UploadBatches;
        break;
    case SGNode::E_DirtyType::OrderingDirty:
        m_rebuild |= E_RebuildFlag::BuildNodes;
        m_rebuild |= E_RebuildFlag::UploadBatches;
        break;
    case SGNode::E_DirtyType::OpacityDirty:
        node->opacityChanged = 1;
        m_rebuild |= E_RebuildFlag::FullRebuild;
        break;
    case SGNode::E_DirtyType::VisibilityDirty:
        node->visibleChanged = 1;
        m_rebuild |= E_RebuildFlag::FullRebuild;
        break;
    case SGNode::E_DirtyType::AllDirty:
        node->geometryChanged = 1;
        node->visibleChanged = 1;
        node->opacityChanged = 1;
        m_rebuild |= E_RebuildFlag::FullRebuild;
        break;
    default:
        break;
    }
}

void SGRenderer::selectRenderNodes()
{
    SGNode::E_NodeType nodeType;
    Node *node;
    for (auto item : m_mapNodes)
    {
        nodeType = item.first->type();
        node = item.second;
        if (!node->visible) continue;

        if (nodeType == SGNode::E_NodeType::ImageNodeType ||
            nodeType == SGNode::E_NodeType::TextNodeType ||
            node->alpha)
        {
            m_alphaNodes.push_back(node);
        }
        else
        {
            m_opacityNodes.push_back(node);
        }
        node->batch = 0;
    }
}

void SGRenderer::buildOpacityBatches()
{
    for (auto itm : m_opaqueBatches)
    {
        m_batchPool.free(itm);
    }
    m_opaqueBatches.clear();

    for (auto node: m_opacityNodes)
    {
        node->batch = 0;
    }
    std::sort(m_opacityNodes.begin(), m_opacityNodes.end(), [](const Node *lop, const Node *rop) ->bool {return lop->z > rop->z; });
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
        if (node->gnode->m_material->share())
        {
            for (int j = i + 1; j < m_opacityNodes.size(); j++)
            {
                Node *nj = m_opacityNodes[j];

                if (nj->batch ||
                    ntype != nj->gnode->type() ||
                    !nj->gnode->m_material->share())
                    continue;

                node->next = nj;
                nj->prev = node;
                nj->batch = 1;
                batch->vbo.verticeNum += nj->gnode->m_geometry->verticeNum();

                node = nj;
                nodenum++;
            }
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

    for (auto node : m_alphaNodes)
    {
        node->batch = 0;
    }
    std::sort(m_alphaNodes.begin(), m_alphaNodes.end(), [](const Node *lop, const Node *rop) ->bool {return lop->z < rop->z; });
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
        if (node->gnode->m_material->share())
        {
            for (int j = i + 1; j < m_alphaNodes.size(); j++)
            {
                Node *nj = m_alphaNodes[j];

                if (nj->batch)
                    continue;

                if (ntype == nj->gnode->type() && nj->gnode->m_material->share())
                {
                    if (overlapBounds.intersects(nj->bounding) &&
                        checkOverlap(i + 1, j - 1, nj->bounding))//not overlap
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

void SGRenderer::uploadBatches()
{
    for (Batch *batch : m_opaqueBatches)
    {
        uploadOneBatch(batch);
    }
    for (Batch *batch : m_alphaBatches)
    {
        uploadOneBatch(batch);
    }
}

void SGRenderer::uploadOneBatch(Batch *batch)
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
    //glClearDepthf(1); // calls glClearDepth() under the hood for desktop OpenGL
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
    assert(n);

    const SGNode *gn = n->gnode;
    
    gn->m_material->syncState();
    SGMaterialShader *shader = m_shrMgr.find(gn->m_material);

    shader->program()->bind();

    shader->updateState(m_view->sceneTransform());

    glBindBuffer(GL_ARRAY_BUFFER, batch->vbo.id);

    auto attrs = gn->m_geometry->attributes();
    for (int i = 0; i < attrs->vparanum; i++)
    {
        glVertexAttribPointer(i, attrs->vas[i].size, GL_FLOAT, GL_FALSE, attrs->vsize, (void*)(attrs->vas[i].offset));
        glEnableVertexAttribArray(i);
    }

    glDrawArrays(gn->m_geometry->renderType(), 0, batch->vbo.verticeNum);
}
