#ifndef SCENEFANTACY_H
#define SCENEFANTACY_H

#include "src/sgscene.h"
#include <QObject>

class SceneFantacy : public QObject, public SGScene
{
    Q_OBJECT
    friend class SGView;
public:
    SceneFantacy();

protected:
    virtual void timerEvent(QTimerEvent *event) override;
    void testAlloc();
    void testVisible(int gen);
    void testUpdate();

    std::vector<SGNode *> m_nodes;
};

#endif // SCENEFANTACY_H
