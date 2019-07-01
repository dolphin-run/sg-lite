#ifndef SCENE_H
#define SCENE_H

#include "src/sgscene.h"
#include <QList>

class Scene : public QObject, public SGScene
{
    Q_OBJECT
    friend class SGView;
public:
    Scene();

protected:
    virtual void timerEvent(QTimerEvent *event) override;
    void testAlloc();

    QList<SGNode *> m_nodes;
};

#endif // SCENE_H
