
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
QT += widgets


include(src/sgcore.pri)
INCLUDEPATH += src

include(example-scene/example-scene.pri)
INCLUDEPATH += example-scene

HEADERS += example-window/qt5widget/view.h example-window/qt5widget/viewinteraction.h

SOURCES += example-window/qt5widget/view.cpp example-window/qt5widget/viewinteraction.cpp example-window/qt5widget/main.cpp
