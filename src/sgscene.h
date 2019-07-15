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
    void itemChanged(SGNode *item);

    //add `item` as a child of `attached` or as a root node when not attached. this scene will take the ownership of `item`.
    //if item has a parent, it will be just ignored.
    //if `attached` specified not in the scene, it will be failed.
    bool addItem(SGNode *item, SGNode *attached = nullptr);

    //disconnect `item` from this scene. ownership will return to the caller.
    //this will change `item` relation-ship from its parent, but keep its children relation-ship.
    bool removeItem(SGNode *item);

    //remove all nodes from this scene and delete it.
    void clear();

protected:
    //just remove without edit inheritation
    void remove(SGNode *item);
    void add(SGNode *item);

    //notify view init
    void addView(SGView* view);

protected:
    std::list<SGView*> m_views;//view obeserver

private:
    SGRootNode m_root;

    std::unordered_set<SGNode*> m_nodes;

    int m_width = 1920;
    int m_height = 1080;
};

#endif // SGSCENE_H
