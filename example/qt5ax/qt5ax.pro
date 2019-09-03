
contains(QT_ARCH, i386) {
    PLATFORM = x86
} else {
    PLATFORM = x64
    ARCH_SUFFIX = 64
}

CONFIG(debug, debug|release) {
    BUILD_SUFFIX = d
} else {
}

CONFIG -= flat
QT += widgets axserver


DESTDIR = $$PWD/../bin/$${PLATFORM}


INCLUDEPATH += ../../include
include(../example-scene/example-scene.pri)
LIBS += -L../../lib/$${PLATFORM} -lsgcore$${BUILD_SUFFIX}

HEADERS += view.h widgetwrap.h
SOURCES += view.cpp widgetwrap.cpp main.cpp
