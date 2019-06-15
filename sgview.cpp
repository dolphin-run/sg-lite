#include "sgview.h"
#include <QScreen>
#include "sgnode.h"
#include "sgscene.h"

//! [1]
SGView::SGView(QWindow *parent)
    : QWindow(parent)
    , m_context(0)
{
    setSurfaceType(QWindow::OpenGLSurface);
}
//! [1]

SGView::~SGView()
{
}

void SGView::initialize()
{
}

void SGView::setScene(SGScene *scene)
{
    m_scene = scene;
}

void SGView::drawItem(SGNode *node, SGFlatColorMaterial* mt)
{
    if(!node) return;
    //...
    
    mt->m_program->setUniformValue(mt->m_matrixUniform, m_scene->m_projection * node->matrix);

    glGenBuffers(1, &mt->m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mt->m_vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(mt->vertices), mt->vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));
    glEnableVertexAttribArray(1);


    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //
    if(node->m_children.size())
    {
        for(SGNode* child: node->m_children)
        {
            drawItem(child, mt);
        }
    }
}

void SGView::render()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT);
    
    SGMaterial *mt1 = SGShaderManager::find(FlatColorMaterialType);
    Q_ASSERT(mt1->type() == E_MaterialType::FlatColorMaterialType);
    SGFlatColorMaterial* mt = dynamic_cast<SGFlatColorMaterial*>(mt1);
    
    mt->m_program->bind();

    glBindBuffer(GL_ARRAY_BUFFER, mt->m_vbo);
    drawItem(m_scene->m_root, mt);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    mt->m_program->release();
}

void SGView::renderLater()
{
    requestUpdate();
}

bool SGView::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::UpdateRequest:
        renderNow();
        return true;
    default:
        return QWindow::event(event);
    }
}

void SGView::resizeEvent(QResizeEvent * event)
{
    //m_projection
}

void SGView::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);

    if (isExposed())
        renderNow();
}

void SGView::renderNow()
{
    if (!isExposed())
        return;

    bool needsInitialize = false;

    if (!m_context) {
        m_context = new QOpenGLContext(this);
        m_context->setFormat(requestedFormat());
        m_context->create();

        needsInitialize = true;
    }

    m_context->makeCurrent(this);

    if (needsInitialize) {
        initializeOpenGLFunctions();
        initialize();
    }

    render();

    m_context->swapBuffers(this);
}
