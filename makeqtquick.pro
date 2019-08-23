
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


include(src/sgcore.pri)
INCLUDEPATH += src

include(example-scene/example-scene.pri)
INCLUDEPATH += example-scene

HEADERS += example-window/qt5quick/fbo.h example-window/qt5quick/fborenderer.h

SOURCES += example-window/qt5quick/fbo.cpp example-window/qt5quick/fborenderer.cpp example-window/qt5quick/main.cpp

OTHER_FILES = example-window/qt5quick/main.qml
