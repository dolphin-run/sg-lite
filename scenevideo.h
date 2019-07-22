#ifndef SCENEVIDEO_H
#define SCENEVIDEO_H

#include "src/sgscene.h"
#include <QList>
#include "singletondevicestream.h"

class SGYuvVideoNode;
class SceneVideo : public QObject, public SGScene, public InterfaceMsgContainer
{
    Q_OBJECT
    friend class SGView;
public:
    SceneVideo();

protected:
    //
    virtual void recvVideoBlob(const PureVideoBlob *video, const SmartDataBlob *data) override;
    virtual void recvCommnBlob(const SmartDataBlob *data) override;
    InterfaceDeviceStream *m_device;

    virtual void timerEvent(QTimerEvent *event) override;
    void testAlloc();

    QList<SGNode *> m_nodes; 
    SGYuvVideoNode *m_videoNode = nullptr;
    SGYuvVideoNode *m_videoNode1 = nullptr;
};

#endif // SCENEVIDEO_H
