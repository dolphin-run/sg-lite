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


DESTDIR = $$PWD/../bin/$${PLATFORM}


INCLUDEPATH += ../../include
include(../example-scene/example-scene.pri)
LIBS += -L../../lib/$${PLATFORM} -lsgcore$${BUILD_SUFFIX}



INCLUDEPATH += 3rdparty/include
LIBS += -L3rdparty/lib$${ARCH_SUFFIX} -lglfw3

HEADERS += view.h

SOURCES += view.cpp main.cpp

