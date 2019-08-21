
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


include(src/sgcore.pri)
INCLUDEPATH += src

include(example-scene/example-scene.pri)
INCLUDEPATH += example-scene

HEADERS += example-window/qt5window/view.h example-window/qt5window/viewinteraction.h

SOURCES += example-window/qt5window/view.cpp example-window/qt5window/viewinteraction.cpp example-window/qt5window/main.cpp
