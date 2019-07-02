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
        matrix.translate(gnode->m_x + gnode->m_width / 2, gnode->m_y + gnode->m_height / 2);
        matrix.scale(gnode->m_width, gnode->m_height);

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
}

void SGRenderer::initializeGL()
{
    initializeOpenGLFunctions();
}

void SGRenderer::sync(const SGRootNode *root)
{
    for (auto itm : root->m_children)
        syncGnode(itm);
}

void SGRenderer::syncGnode(const SGNode * node)
{
    Node *n = new Node; n->gnode = node;

    m_mapNodes.insert(std::make_pair(node, n));

    for (auto child : node->m_children)
    {
        syncGnode(child);
    }
}

void SGRenderer::render()
{
    selectRenderNodes();
    buildOpacityBatches();

    uploadBatches();

    const qreal retinaScale = m_view->devicePixelRatio();
    glViewport(0, 0, m_view->width() * retinaScale, m_view->height() * retinaScale);
    glClear(GL_COLOR_BUFFER_BIT); 

    //to do...
    //will be move to other function
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    renderBatches();
}

void SGRenderer::selectRenderNodes()
{
    m_opacityNodes.clear();
    m_alphaNodes.clear();
    for (auto itm : m_mapNodes)
    {
        //if (node->type() == SGNode::E_NodeType::RectangleNodeType)
        if (itm.first->visible())
        {
            itm.second->updateMatrix();
            itm.second->batch = 0;
            m_opacityNodes.push_back(itm.second);
        }
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
        int nodenum = 1;
        for (int j = i + 1; j < m_opacityNodes.size(); j++)
        {
            Node *nj = m_opacityNodes[j];

            if (nj->batch ||
                ntype != nj->gnode->type())
                continue;

            node->next = nj;
            nj->prev = node;
            nj->batch = 1;

            node = nj;
            nodenum++;
        }
        node->next = nullptr;

        batch->vbo.verticeNum = nodenum * node->gnode->m_geometry->verticeNum() + (nodenum-1) * 2;
        batch->vbo.size = batch->vbo.verticeNum * node->gnode->m_geometry->verticeSize();

        m_opaqueBatches.push_back(batch);
    }
}

void SGRenderer::map(Buffer *buffer)
{
    //to do ... add memory pool

    if (buffer->data)
        delete buffer->data;
    
    buffer->data = new char[buffer->size];
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
        map(&batch->vbo);

        //write data
        Node *node = batch->firstChild;
        char* pd = (char*)batch->vbo.data;
        while (node)
        {
            SGGeometry *gg = node->gnode->m_geometry;

            char* pdmain = pd;
            if (node->prev)
            {
                pdmain = pd + gg->verticeSize();
            }
            memcpy(pdmain, gg->data(), gg->size());

            //apply transformation
            VertexAsPointColor *pvr = (VertexAsPointColor *)pdmain;
            for (int i = 0; i < gg->verticeNum(); i++)
            {
                PointF *pt = (PointF *)(pvr+i);
                pt->map(node->matrix);
            }

            if (node->prev)
            {
                memcpy(pd, pdmain, gg->verticeSize());
                pd += gg->verticeSize();
            }
            pd += gg->size();

            node = node->next;

            if (node)
            {
                //add degenerate node
                memcpy(pd, pd - gg->verticeSize(), gg->verticeSize());
                pd += gg->verticeSize();
            }
        }

        //end write

        unmap(&batch->vbo);
    }
}

void SGRenderer::renderBatches()
{
    for (Batch *batch : m_opaqueBatches)
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


        glDrawArrays(GL_TRIANGLE_STRIP, 0, batch->vbo.verticeNum);
    }
}