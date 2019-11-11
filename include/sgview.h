#ifndef SGVIEW_H
#define SGVIEW_H

#include "sgstruct.h"
#include "sgnode.h"


/*
** subclass should supply OpenGL Render Context as needed.
*/

class SGScene;
class SGViewCamera;
class SGRenderer;
class SG_DECL_EXPORT SGView
{
    SG_DISABLE_COPY(SGView);
    friend class SGScene;
public:
    explicit SGView();
    virtual ~SGView();

    void attach(SGScene *scene);
    void detach();

    //`camera` will be owned by this view, it will be delete auto.
    SGViewCamera* setViewCamera(SGViewCamera *camera);
    SGViewCamera *viewCamera();

    const RectF &viewRect() const;

    Point mapToScene(const Point &vp) const;

    int widthPx() const;
    int heightPx() const;

protected:
    //call from Scene, notify renderer to rebuild.
    virtual void itemAdded(SGNode *item);
    virtual void itemDeleted(SGNode *item);
    virtual void itemChanged(SGNode *item, SGNode::E_DirtyType changes);
    virtual void itemsCleaned();

    //Viewer(desktop-window depends) call when the view size changed.
    virtual void setSize(int width, int height);
    virtual void setSizePx(int wpx, int hpx);

private:
    void sceneChanged();
    void sizeChanged();
    void sizePxChanged();
    void cameraChanged();

protected:
    bool m_renderInited = false;
    virtual bool initializeRender();
    virtual void renderFrame();

protected:
    SGViewCamera *m_camera = nullptr;
    SGScene *m_scene = nullptr;
    SGRenderer *m_render = nullptr;

    int m_width;
    int m_height;
    int m_widthPx;
    int m_heightPx;
};

#endif // SGVIEW_H
