#ifndef SCENEQUICKITEMRENDERER_H
#define SCENEQUICKITEMRENDERER_H

#include <QQuickFramebufferObject>
#include <QOpenGLFramebufferObject>
#include <qquickwindow.h>

#include "sgview.h"
#include "scene.h"
#include "scenefantacy.h"
#include "scenevideo.h"

class SceneQuickItemRenderer : public QQuickFramebufferObject::Renderer, public SGView
{
public:
    SceneQuickItemRenderer()
    {
        attach(&m_scene);
    }

    void updateScene(const char* psrc) {

    }

    void synchronize(QQuickFramebufferObject*item){
        m_hqItem = dynamic_cast<QQuickItem*>(item);

        auto width = m_hqItem->width();
        auto height = m_hqItem->height();
        if (width != m_width || height != m_height)
        {
            int radio = m_hqItem->window()->devicePixelRatio();
            setSize(width, height);
            setSizePx(width*radio, height*radio);
        }
        m_width = width;
        m_height = height;
    }

    virtual void render() override {
        //m_hqItem->window()->resetOpenGLState();
        updateScene(nullptr);
        SGView::renderFrame();
        //update();
    }

    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) {
        QOpenGLFramebufferObjectFormat format;
        format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        format.setSamples(4);
        return new QOpenGLFramebufferObject(size, format);
    }

private:
    //VideoCoreRenderer view;
    QQuickItem* m_hqItem = nullptr;
    int m_width = 1920;
    int m_height = 1080;
    QColor m_background;

    //mutable Scene m_scene;
    //SceneFantacy m_scene;
    SceneVideo m_scene;
};

#endif // SCENEQUICKITEMRENDERER_H
