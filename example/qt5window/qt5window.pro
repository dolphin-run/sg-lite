
contains(QT_ARCH, i386) {
    PLATFORM = x86
} else {
    PLATFORM = x64
    ARCH_SUFFIX = 64
}

CONFIG(debug, debug|release) {
    BUILD_SUFFIX = d
    CONFIG += console
} else {
}

CONFIG -= flat

DESTDIR = $$PWD/../bin/$${PLATFORM}


INCLUDEPATH += ../../include
include(../example-scene/example-scene.pri)
LIBS += -L../../lib/$${PLATFORM} -lsgcore$${BUILD_SUFFIX}

HEADERS += view.h viewinteraction.h

SOURCES += view.cpp viewinteraction.cpp main.cpp
