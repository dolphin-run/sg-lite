#ifndef WIDGETWRAP_H
#define WIDGETWRAP_H

#include "../qt5widget/view.h"
#include <QWidget>
#include <QAxBindable>

class WidgetWrap : public QWidget,
              public QAxBindable
{
    Q_OBJECT
    Q_CLASSINFO("ClassID",     "{6E75B0F6-18B9-49B0-93A1-2C26CF72A38B}")
    Q_CLASSINFO("InterfaceID", "{0B132651-EB25-4AD9-8F62-ABB307AC55DA}")
    Q_CLASSINFO("EventsID",    "{E10630E0-4FA3-41FC-8994-34E381E96D48}")
    //! [0] //! [1]

public:
    explicit WidgetWrap(QWidget *parent = nullptr);
    QAxAggregated *createAggregate();

public slots:
    void                setXRotation(int degrees);
    void                setYRotation(int degrees);
    void                setZRotation(int degrees);
    void start();
    void setIp(const QString &ip);

private:
    View *m_videowidget;
};

#endif // WIDGETWRAP_H
