#ifndef SGSCENE_H
#define SGSCENE_H

#include "sgnode.h"
#include "sgglobal.h"

class SGRootNode;
class SGView;
class SGScenePrivate;
class SG_DECL_EXPORT SGScene
{
    SG_DISABLE_COPY(SGScene);
    SG_DECLARE_PRIVATE(SGScene);
    friend class SGView;
    friend class SGNode;
public:
    SGScene();
    virtual ~SGScene();

    //add `item` as a child of `attached` or as a root node when not attached. this scene will take the ownership of `item`.
    //if item has a parent, it will be just ignored.
    //if `attached` specified not in the scene, it will be failed.
    bool addItem(SGNode *item, SGNode *attached = nullptr);

    //disconnect `item` from this scene. ownership will return to the caller.
    //this will set `item` parent to null, but keep its children relation-ship.
    bool removeItem(SGNode *item);

    //remove all nodes from this scene and delete it.
    void clear();

    void setSize(int wid, int hei);
    float width() const;
    float height() const;
    const SGRootNode *rootNode() const;

protected:
    //signals when scene changed.
    void sizeChanged();
    void itemAdded(SGNode *item);
    void itemDeleted(SGNode *item);
    void itemChanged(SGNode *item, SGNode::E_DirtyType changes);
    void itemsCleaned();

protected:
    //add or remove item and its all generations from this scene
    //item tree relation-ship keep the same
    void remove(SGNode *item);
    void add(SGNode *item);

    //notify view init
    void addView(SGView* view);
    void delView(SGView* view);

    SGScenePrivate *d_ptr;
};

#endif // SGSCENE_H
