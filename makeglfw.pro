TEMPLATE = app
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt flat


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


INCLUDEPATH += example-window/glfw/3rdparty/include
LIBS += -Lexample-window/glfw/3rdparty/lib$${ARCH_SUFFIX} -lglfw3


include(src/sgcore.pri)
INCLUDEPATH += src

include(example-scene/example-scene.pri)
INCLUDEPATH += example-scene

HEADERS += example-window/glfw/view.h

SOURCES += example-window/glfw/view.cpp example-window/glfw/main.cpp

