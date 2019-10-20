#ifndef SGVIEWCAMERA_H
#define SGVIEWCAMERA_H

#include "sgstruct.h"
#include "sgglobal.h"

class SGViewCameraPrivate;
class SG_DECL_EXPORT SGViewCamera
{
    SG_DECLARE_PRIVATE(SGViewCamera);
    friend class SGView;
public:
    SGViewCamera();
    ~SGViewCamera();
    void moveCenter();
    void move(float x, float y);
    void scale(float s);
    void setFillMode(bool strench);
    void reset();

    const float* transform() const;
    const RectF &viewRect() const;
    Point mapToScene(const Point & vp) const;

protected:
    void setSceneSize(float wid, float hei);
    void setViewSize(float wid, float hei);
    void update();

    SGViewCameraPrivate *d_ptr;
};

#endif // SGVIEWCAMERA_H
