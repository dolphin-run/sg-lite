include(src/sgcore.pri)
INCLUDEPATH += $$PWD\src
CONFIG -= flat

HEADERS += scene.h

SOURCES += \
    scene.cpp \
    main.cpp
