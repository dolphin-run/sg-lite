#include "widgetwrap.h"
#include <QHBoxLayout>
#include <QAxAggregated>
#include <QUuid>
#include <objsafe.h>

#include "sgscene.h"
#include <thread>
#include <atomic>
#include <list>

#include "view.h"
#include "node/sgrectanglenode.h"
#include "sgviewcamera.h"
#include "scenefantacy.h"

class Scene : public SGScene
{
public:
    Scene()
    {
        m_videoNode = new SGYuvVideoNode;
    }
    ~Scene()
    {
        if (m_videoNode)
            delete m_videoNode;
    }
    void setYuv(const unsigned char *src)
    {
        m_videoNode->setYuv(src);
    }

protected:
    SGYuvVideoNode *m_videoNode = nullptr;
};

class ObjectSafetyImpl : public QAxAggregated,
    public IObjectSafety
{
public:
    //! [1] //! [2]
    explicit ObjectSafetyImpl() {}

    long queryInterface(const QUuid &iid, void **iface)
    {
        *iface = nullptr;
        if (iid == IID_IObjectSafety)
            *iface = (IObjectSafety*)this;
        else
            return E_NOINTERFACE;

        AddRef();
        return S_OK;
    }

    //! [2] //! [3]
    QAXAGG_IUNKNOWN;

    //! [3] //! [4]
    HRESULT WINAPI GetInterfaceSafetyOptions(REFIID riid, DWORD *pdwSupportedOptions, DWORD *pdwEnabledOptions)
    {
        *pdwSupportedOptions = INTERFACESAFE_FOR_UNTRUSTED_DATA | INTERFACESAFE_FOR_UNTRUSTED_CALLER;
        *pdwEnabledOptions = INTERFACESAFE_FOR_UNTRUSTED_DATA | INTERFACESAFE_FOR_UNTRUSTED_CALLER;
        return S_OK;
    }

    HRESULT WINAPI SetInterfaceSafetyOptions(REFIID riid, DWORD pdwSupportedOptions, DWORD pdwEnabledOptions)
    {
        return S_OK;
    }
};

WidgetWrap::WidgetWrap(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);

    m_view = new View(this);
    m_scene = new SceneFantacy;
    m_view->attach(m_scene);
    m_view->viewCamera()->setFillMode(true);

    layout->addWidget(m_view);
}

WidgetWrap::~WidgetWrap()
{
    delete m_scene;
}


void WidgetWrap::start()
{
    //m_videowidget->start();
}

void WidgetWrap::setIp(const QString &ip)
{
    //m_videowidget->setIp(ip);
}

/*!
Set the rotation angle of the object to \e degrees around the X axis.
*/

void WidgetWrap::setXRotation(int degrees)
{
    //m_videowidget->m_xRot = (GLfloat)(degrees % 360);
    //m_videowidget->update();
}


/*!
Set the rotation angle of the object to \e degrees around the Y axis.
*/

void WidgetWrap::setYRotation(int degrees)
{
    //m_videowidget->m_yRot = (GLfloat)(degrees % 360);
    //m_videowidget->update();
}


/*!
Set the rotation angle of the object to \e degrees around the Z axis.
*/

void WidgetWrap::setZRotation(int degrees)
{
    //m_videowidget->m_zRot = (GLfloat)(degrees % 360);
    //m_videowidget->update();
}

QAxAggregated *WidgetWrap::createAggregate()
{
    return new ObjectSafetyImpl();
}
//! [5]
