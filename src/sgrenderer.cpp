#include "sgrenderer.h"
#include <QOpenGLTexture>
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
    
    g_ms.insert(std::make_pair(type, shader));
    return shader;
}

SGRenderer::SGRenderer(SGView *view) :
    m_view(view)
{
    //test_Alloc();
    m_projection.setToIdentity();
    m_projection.ortho(0, 1920, 1080, 0, -1, 1);
}

void SGRenderer::initializeGL()
{
    initializeOpenGLFunctions();

    QImage localImg("F:\\doc\\mk\\VAT\\src\\glmv\\src\\tools\\resource-gen\\allentities.png");
    Q_ASSERT(!localImg.isNull());
    m_tex = new QOpenGLTexture(localImg);
    m_tex->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    m_tex->setMagnificationFilter(QOpenGLTexture::Linear);
    m_tex->setWrapMode(QOpenGLTexture::Repeat);
}

void SGRenderer::render(const SGRootNode *root)
{
    static bool init = false;
    if (!init)
    {
        init = true;
        findRenderNodesFromScratch(root);
        buildOpacityBatches();
    }

    uploadBatches();

    const qreal retinaScale = m_view->devicePixelRatio();
    glViewport(0, 0, m_view->width() * retinaScale, m_view->height() * retinaScale);
    glClear(GL_COLOR_BUFFER_BIT);

    renderBatches();
}

void SGRenderer::findRenderNodesFromScratch(const SGNode * root)
{
    assert(root->type() == SGNode::E_NodeType::RootNodeType);

    for (auto itm : m_opacityNodes)
    {
        delete itm;
    }
    m_opacityNodes.clear();
    m_alphaNodes.clear();
    for (auto itm : root->m_children)
        findRenderNodes(itm);
}

void SGRenderer::findRenderNodes(const SGNode * node)
{
    //if (node->type() == SGNode::E_NodeType::RectangleNodeType)
    {
        if (node->visible())
        {
            Node *n = new Node; n->gnode = node; n->updateMatrix();
            n->batch = 0;
            m_opacityNodes.push_back(n);

            for (auto child : node->m_children)
            {
                findRenderNodes(child);
            }
        }
    }
}

void SGRenderer::buildOpacityBatches()
{
    for (auto itm : m_opaqueBatches)
    {
        delete itm;
    }
    m_opaqueBatches.clear();

    for (int i = 0; i < m_opacityNodes.size(); i++)
    {
        Node *node = m_opacityNodes[i];

        if (node->batch) continue;

        Batch *batch = new Batch;
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
    if (!buffer->data) buffer->data = new char[buffer->size];
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
                Point *pt = (Point *)(pvr+i);
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
        m_tex->bind();

        shader->updateState(m_projection);

        glBindBuffer(GL_ARRAY_BUFFER, batch->vbo.id);

        auto attrs = gn->m_geometry->attributes();
        for (int i = 0; i < attrs->vparanum; i++)
        {
            glVertexAttribPointer(i, attrs->vas[i].size, GL_FLOAT, GL_FALSE, attrs->vsize, (void*)(attrs->vas[i].offset));
            glEnableVertexAttribArray(i);
        }


        glDrawArrays(GL_TRIANGLE_STRIP, 0, batch->vbo.verticeNum);
        //break;
    }
}


void SGRenderer::test_Alloc()
{
    //Add SGNode
    SGRectangleNode *sn0 = new SGRectangleNode;
    sn0->setX(100);
    sn0->setY(50);
    sn0->setWidth(400);
    sn0->setHeight(100);
    sn0->setColor(0xff0000);
    SGRectangleNode *sn1 = new SGRectangleNode;
    sn1->setX(800);
    sn1->setY(200);
    sn1->setWidth(400);
    sn1->setHeight(100);
    sn1->setColor(0x00ff00);
    SGRectangleNode *sn2 = new SGRectangleNode;
    sn2->setX(300);
    sn2->setY(250);
    sn2->setWidth(400);
    sn2->setHeight(100);
    sn2->setColor(0x0000ff);
    SGRectangleNode *sn3 = new SGRectangleNode;
    sn3->setX(1300);
    sn3->setY(750);
    sn3->setWidth(400);
    sn3->setHeight(100);

    //build Node&Batch from SGNode
    Node *n0 = new Node; n0->gnode = sn0; n0->updateMatrix();
    Node *n1 = new Node; n1->gnode = sn1; n1->updateMatrix();
    Node *n2 = new Node; n2->gnode = sn2; n2->updateMatrix();
    n2->next = n1;
    n1->prev = n2;
    n1->next = n0;
    n0->prev = n1;

    Batch *b0 = new Batch;
    b0->firstChild = n2;
    b0->vbo.verticeNum = 3 * 4 + 2 * 2;
    b0->vbo.size = b0->vbo.verticeNum * n2->gnode->m_geometry->verticeSize();

    //
    Node *n3 = new Node; n3->gnode = sn3; n3->updateMatrix();

    Batch *b1 = new Batch;
    b1->firstChild = n3;
    b1->vbo.verticeNum = 1 * 4;
    b1->vbo.size = b1->vbo.verticeNum * n3->gnode->m_geometry->verticeSize();


    //add some ring
    SGCircularNode *sn10 = new SGCircularNode;
    sn10->setX(100);
    sn10->setY(550);
    sn10->setWidth(400);
    sn10->setHeight(400);
    sn10->setColor(0x1C86EE);
    SGCircularNode *sn11 = new SGCircularNode;
    sn11->setX(0);
    sn11->setY(0);
    sn11->setWidth(400);
    sn11->setHeight(400);
    sn11->setColor(0xCD5C5C);

    Node *n10 = new Node; n10->gnode = sn10; n10->updateMatrix();
    Node *n11 = new Node; n11->gnode = sn11; n11->updateMatrix();
    n10->next = n11;
    n11->prev = n10;

    Batch *b10 = new Batch;
    b10->firstChild = n10;
    b10->vbo.verticeNum = 2 * sn10->m_geometry->verticeNum() + 1 * 2;
    b10->vbo.size = b10->vbo.verticeNum * n10->gnode->m_geometry->verticeSize();

    //add image
    SGImageNode *sn20 = new SGImageNode;
    sn20->setX(1500);
    sn20->setY(0);
    sn20->setWidth(160);
    sn20->setHeight(160);
    sn20->setImage(5);
    SGImageNode *sn21 = new SGImageNode;
    sn21->setX(1500);
    sn21->setY(600);
    sn21->setWidth(80);
    sn21->setHeight(80);
    sn21->setImage(15);

    Node *n20 = new Node; n20->gnode = sn20; n20->updateMatrix();
    Node *n21 = new Node; n21->gnode = sn21; n21->updateMatrix();
    n20->next = n21;
    n21->prev = n20;

    Batch *b20 = new Batch;
    b20->firstChild = n20;
    b20->vbo.verticeNum = 2 * sn20->m_geometry->verticeNum() + 1 * 2;
    b20->vbo.size = b20->vbo.verticeNum * n20->gnode->m_geometry->verticeSize();

    //
    m_opaqueBatches.push_back(b0);
    m_opaqueBatches.push_back(b1);
    m_opaqueBatches.push_back(b10);
    m_opaqueBatches.push_back(b20);
}