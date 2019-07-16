#ifndef SGSCENE_H
#define SGSCENE_H

#include "sgnode.h"
#include <unordered_set>

class SGView;
class SGScene
{
    friend class SGView;
public:
    SGScene();

    //to do...classify in the future.
    void sizeChanged();
    void itemAdded(SGNode *item);
    void itemDeleted(SGNode *item);
    void itemChanged(SGNode *item, SGNode::E_DirtyType changes);

    //add `item` as a child of `attached` or as a root node when not attached. this scene will take the ownership of `item`.
    //if item has a parent, it will be just ignored.
    //if `attached` specified not in the scene, it will be failed.
    bool addItem(SGNode *item, SGNode *attached = nullptr);

    //disconnect `item` from this scene. ownership will return to the caller.
    //this will set `item` parent to null, but keep its children relation-ship.
    bool removeItem(SGNode *item);

    //remove all nodes from this scene and delete it.
    void clear();

protected:
    //add or remove item and its all generations from this scene
    //item tree relation-ship keep the same
    void remove(SGNode *item);
    void add(SGNode *item);

    //notify view init
    void addView(SGView* view);

protected:
    std::list<SGView*> m_views;//view obeserver

private:
    SGRootNode m_root;

    //use list, it can store parent and children from first to end.
    std::list<SGNode*> m_nodes;

    int m_width = 1920;
    int m_height = 1080;
};

#endif // SGSCENE_H
