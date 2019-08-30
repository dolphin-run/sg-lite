
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
DESTDIR = $$PWD/bin/$${PLATFORM}


INCLUDEPATH += include

include(example-scene/example-scene.pri)
INCLUDEPATH += example-scene

LIBS += -Llib/$${PLATFORM} -lsgcore$${BUILD_SUFFIX}
HEADERS += example-window/qt5widget/view.h example-window/qt5widget/viewinteraction.h

SOURCES += example-window/qt5widget/view.cpp example-window/qt5widget/viewinteraction.cpp example-window/qt5widget/main.cpp
