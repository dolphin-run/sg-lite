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

DEFINES += BUILDING_PLUGIN

TARGET = sgcore$${BUILD_SUFFIX}
DESTDIR = $$PWD/../bin/$${PLATFORM}

include(sgcore.pri)
