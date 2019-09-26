TEMPLATE = lib
CONFIG += c++11 plugin
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

DEFINES += SG_BUILDING_PLUGIN SG_GL_GLAD

TARGET = sgcore$${BUILD_SUFFIX}
DESTDIR = $$PWD/../lib/$${PLATFORM}

include(sgcore.pri)

target.files += \
    $$PWD/sgglobal.h \
    $$PWD/sgstruct.h \
    $$PWD/sgnode.h \
    $$PWD/sgview.h \
    $$PWD/sgviewcamera.h \
    $$PWD/sgscene.h
target.path += $$PWD/../include
INSTALLS += target

targetnode.files += \
    $$PWD/node/sgrectanglenode.h \
    $$PWD/node/sgcircularnode.h \
    $$PWD/node/sgtextnode.h
targetnode.path += $$PWD/../include/node
INSTALLS += targetnode
