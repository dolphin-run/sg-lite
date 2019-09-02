#include <QSurfaceFormat>
#include <QApplication>
#include <QVBoxLayout>
#include <QAxFactory>

#include "example-window/qt5ax/widgetwrap.h"

QAXFACTORY_BEGIN(
    "{2c3c183a-eeda-41a4-896e-3d9c12c3577d}", // type library ID
    "{83e16271-6480-45d5-aaf1-3f40b7661ae4}") // application ID
    QAXCLASS(WidgetWrap)
QAXFACTORY_END()

int main(int argc, char **argv)
{
    QSurfaceFormat format;
    format.setSamples(4);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication app(argc, argv);

    if (!QAxFactory::isServer()) {
        WidgetWrap w;
        w.resize(400, 350);
        w.show();
        return app.exec();
    }

    return app.exec();
}
