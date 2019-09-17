
contains(QT_ARCH, i386) {
    PLATFORM = x86
} else {
    PLATFORM = x64
}

CONFIG(debug, debug|release) {
    BUILD_SUFFIX = d
    CONFIG += console
} else {
}

CONFIG -= flat
QT += widgets

DESTDIR = $$PWD/../../lib/$${PLATFORM}


INCLUDEPATH += ../../src
include(../example-scene/example-scene.pri)
LIBS += -L../../lib/$${PLATFORM} -lsgcore$${BUILD_SUFFIX}

HEADERS += view.h viewinteraction.h \
    mainview.h
SOURCES += view.cpp viewinteraction.cpp main.cpp \
    mainview.cpp

FORMS += \
    mainview.ui
