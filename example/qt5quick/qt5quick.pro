
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

QT += qml quick quickcontrols2


CONFIG -= flat
DEFINES += USERVIEW_HAS_VERTICAL_MIRROR


DESTDIR = $$PWD/../bin/$${PLATFORM}


INCLUDEPATH += ../../include
include(../example-scene/example-scene.pri)
LIBS += -L../../lib/$${PLATFORM} -lsgcore$${BUILD_SUFFIX}

HEADERS += fbo.h fborenderer.h

SOURCES += fbo.cpp fborenderer.cpp main.cpp

OTHER_FILES = ../assets/qml/main.qml
