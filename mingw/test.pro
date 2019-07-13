TEMPLATE = app
CONFIG += console c++11
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


INCLUDEPATH += .. ../src

INCLUDEPATH += 3rdparty/include
LIBS += -L3rdparty/lib$${ARCH_SUFFIX} -lglfw3


include(../src/sgcore.pri)

HEADERS += scene.h view.h

SOURCES += main.cpp scene.cpp view.cpp

