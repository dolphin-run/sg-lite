#include "sgrenderer.h"

SGRenderer::SGRenderer()
{
    initializeOpenGLFunctions();

    test_Alloc();
}

void SGRenderer::render()
{
    for (Batch *batch : m_opaqueBatches)
    {
        map(&batch->vbo);

        //write data

        //end write

        unmap(&batch->vbo);
    }

    renderBatches();
}

void SGRenderer::buildOpacityBatches()
{
    for (Batch *batch : m_opaqueBatches)
    {
        batch->vbo.size = 4;
    }
}

void SGRenderer::map(Buffer *buffer)
{
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

void SGRenderer::renderBatches()
{
    for (Batch *batch : m_opaqueBatches)
    {
        Node *n = batch->firstChild;
        Q_ASSERT(n);
        
        SGNode *gn = n->node;

        glBindBuffer(GL_ARRAY_BUFFER, batch->vbo.id);

        const Buffer *indexBuf = &batch->vbo;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuf->id);

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));
        glEnableVertexAttribArray(1);


        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
}


void SGRenderer::test_Alloc()
{
    Node *n0 = new Node;
    n0->sibling = 0;
    Node *n1 = new Node;
    n1->sibling = n0;
    Node *n2 = new Node;
    n2->sibling = n1;

    Node *n3 = new Node;

    Batch *b0 = new Batch;
    b0->firstChild = n2;

    Batch *b1 = new Batch;
    b1->firstChild = n3;
}