
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

include(src/sgcore.pri)
INCLUDEPATH += $$PWD/src

CONFIG -= flat

HEADERS += scene.h scenevideo.h view.h scenefantacy.h viewinteraction.h singletondevicestream.h

SOURCES += main.cpp scene.cpp scenevideo.cpp view.cpp scenefantacy.cpp viewinteraction.cpp singletondevicestream.cpp
