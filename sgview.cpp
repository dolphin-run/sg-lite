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

static const char *vertexShaderSource =
    "attribute highp vec4 posAttr;\n"
    "attribute lowp vec4 colAttr;\n"
    "varying lowp vec4 col;\n"
    "uniform highp mat4 matrix;\n"
    "void main() {\n"
    "   col = colAttr;\n"
    "   gl_Position = matrix * posAttr;\n"
    "}\n";

static const char *fragmentShaderSource =
    "varying lowp vec4 col;\n"
    "void main() {\n"
    "   gl_FragColor = col;\n"
    "}\n";

void SGView::initialize()
{
    for(auto m: g_Materials)
    {
        m->build();
    }
}

void SGView::setScene(SGScene *scene)
{
    m_scene = scene;
}

void SGView::drawItem(SGNode *node, SGFlatColorMaterial* mt)
{
    if(!node) return;
    //...

    QMatrix4x4 matrix;
    matrix.ortho(0, m_scene->m_width, m_scene->m_height, 0, -1, 1);
    matrix.translate(node->x + node->width / 2, node->y + node->height / 2);
    matrix.scale(node->width,node->height);
    
    mt->m_program->setUniformValue(mt->m_matrixUniform, matrix);


    glDrawArrays(GL_TRIANGLES, 0, 6);

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
    
    Q_ASSERT(m_scene->m_root->m_material->type() == E_MaterialType::FlatColorMaterialType);
    SGFlatColorMaterial* mt = dynamic_cast<SGFlatColorMaterial*>(m_scene->m_root->m_material);
    
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
