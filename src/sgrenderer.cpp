#include "sgrenderer.h"
#include <QOpenGLTexture>
#include "node/sgcircularnode.h"
#include "node/sgrectanglenode.h"

SGMaterialShader * SGShaderManager::find(SGMaterial *material)
{
    E_MaterialType type = material->type();
    if (g_ms.count(type)) return g_ms[type];

    SGMaterialShader *shader = material->createShader();
    
    g_ms.insert(std::make_pair(type, shader));
    return shader;
}

SGRenderer::SGRenderer()
{
    test_Alloc();
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

void SGRenderer::render(int width, int height)
{
    buildOpacityBatches();

    uploadBatches();

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    renderBatches();
}

void SGRenderer::buildOpacityBatches()
{
    //...
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
                Pt *pt = (Pt *)(pvr+i);
                pt->map(node->gnode->matrix);
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
        
        SGNode *gn = n->gnode;

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
    }
}


void SGRenderer::test_Alloc()
{
    //Add SGNode
    SGRectangleNode *sn0 = new SGRectangleNode;
    sn0->x = 100;
    sn0->y = 50;
    sn0->width = 400;
    sn0->height = 100;
    sn0->setColor(0xff0000);
    sn0->updateMatrix();
    SGRectangleNode *sn1 = new SGRectangleNode;
    sn1->x = 800;
    sn1->y = 200;
    sn1->width = 400;
    sn1->height = 100;
    sn1->setColor(0x00ff00);
    sn1->updateMatrix();
    SGRectangleNode *sn2 = new SGRectangleNode;
    sn2->x = 300;
    sn2->y = 250;
    sn2->width = 400;
    sn2->height = 100;
    sn2->setColor(0x0000ff);
    sn2->updateMatrix();
    SGRectangleNode *sn3 = new SGRectangleNode;
    sn3->x = 1300;
    sn3->y = 750;
    sn3->width = 400;
    sn3->height = 100;
    sn3->updateMatrix();

    //build Node&Batch from SGNode
    Node *n0 = new Node; n0->gnode = sn0;
    Node *n1 = new Node; n1->gnode = sn1;
    Node *n2 = new Node; n2->gnode = sn2;
    n2->next = n1;
    n1->prev = n2;
    n1->next = n0;
    n0->prev = n1;

    Batch *b0 = new Batch;
    b0->firstChild = n2;
    b0->vbo.verticeNum = 3 * 4 + 2 * 2;
    b0->vbo.size = b0->vbo.verticeNum * n2->gnode->m_geometry->verticeSize();

    //
    Node *n3 = new Node; n3->gnode = sn3;

    Batch *b1 = new Batch;
    b1->firstChild = n3;
    b1->vbo.verticeNum = 1 * 4;
    b1->vbo.size = b1->vbo.verticeNum * n3->gnode->m_geometry->verticeSize();


    //add some ring
    SGCircularNode *sn10 = new SGCircularNode;
    sn10->x = 100;
    sn10->y = 550;
    sn10->width = 400;
    sn10->height = 400;
    sn10->setColor(0x1C86EE);
    sn10->updateMatrix();
    SGCircularNode *sn11 = new SGCircularNode;
    sn11->x = 0;
    sn11->y = 0;
    sn11->width = 400;
    sn11->height = 400;
    sn11->setColor(0xCD5C5C);
    sn11->updateMatrix();

    Node *n10 = new Node; n10->gnode = sn10;
    Node *n11 = new Node; n11->gnode = sn11;
    n10->next = n11;
    n11->prev = n10;

    Batch *b10 = new Batch;
    b10->firstChild = n10;
    b10->vbo.verticeNum = 2 * sn10->m_geometry->verticeNum() + 1 * 2;
    b10->vbo.size = b10->vbo.verticeNum * n10->gnode->m_geometry->verticeSize();

    //add image
    SGImageNode *sn20 = new SGImageNode;
    sn20->x = 1500;
    sn20->y = 0;
    sn20->width = 160;
    sn20->height = 160;
    sn20->setImage(5);
    sn20->updateMatrix();
    SGImageNode *sn21 = new SGImageNode;
    sn21->x = 1500;
    sn21->y = 600;
    sn21->width = 80;
    sn21->height = 80;
    sn21->setImage(15);
    sn21->updateMatrix();

    Node *n20 = new Node; n20->gnode = sn20;
    Node *n21 = new Node; n21->gnode = sn21;
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