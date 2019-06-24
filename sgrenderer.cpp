#include "sgrenderer.h"

SGRenderer::SGRenderer()
{
    test_Alloc();
    m_projection.setToIdentity();
    m_projection.ortho(0, 1920, 1080, 0, -1, 1);
}

void SGRenderer::initializeGL()
{
    initializeOpenGLFunctions();
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
            VertexAsColorRectangle *pvr = (VertexAsColorRectangle *)pdmain;
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

        SGMaterial *mt1 = SGShaderManager::find(FlatColorMaterialType);
        mt1->active();
        Q_ASSERT(mt1->type() == E_MaterialType::FlatColorMaterialType);
        SGFlatColorMaterial* mt = dynamic_cast<SGFlatColorMaterial*>(mt1);

        bool ret = mt->m_program->bind();


        mt->m_program->setUniformValue(mt->m_matrixUniform, m_projection);

        

        glBindBuffer(GL_ARRAY_BUFFER, batch->vbo.id);

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));
        glEnableVertexAttribArray(1);


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
    b0->vbo.size = b0->vbo.verticeNum * 8 * sizeof(GLfloat);

    //
    Node *n3 = new Node; n3->gnode = sn3;

    Batch *b1 = new Batch;
    b1->firstChild = n3;
    b1->vbo.verticeNum = 1 * 4;
    b1->vbo.size = b1->vbo.verticeNum * 8 * sizeof(GLfloat);

    //
    m_opaqueBatches.push_back(b0);
    m_opaqueBatches.push_back(b1);
}