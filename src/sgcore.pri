INCLUDEPATH += $$PWD/common/include
LIBS += -L$$PWD/common/lib$${ARCH_SUFFIX} -lfreetype$${BUILD_SUFFIX}

SOURCES += \
    $$PWD/sgnode.cpp \
    $$PWD/sgstruct.cpp \
    $$PWD/sgrenderer.cpp \
    $$PWD/sgscene.cpp \
    $$PWD/sgview.cpp \
    $$PWD/sgmaterial.cpp \
    $$PWD/sggeometry.cpp \
    $$PWD/geometry/sgrectanglegeometry.cpp \
    $$PWD/geometry/sgcirculargeometry.cpp \
    $$PWD/material/sgcolormaterial.cpp \
    $$PWD/material/sgtexturematerial.cpp \
    $$PWD/material/sgglyphmaterial.cpp \
    $$PWD/node/sgrectanglenode.cpp \
    $$PWD/node/sgcircularnode.cpp \
    $$PWD/node/sgtextnode.cpp \
    $$PWD/util/sgtexture.cpp \
    $$PWD/util/sgimagetexture.cpp \
    $$PWD/util/sgglyphtexture.cpp \
    $$PWD/viewinteraction.cpp

HEADERS += \
    $$PWD/sgnode.h \
    $$PWD/sgstruct.h \
    $$PWD/sgrenderer.h \
    $$PWD/sgscene.h \
    $$PWD/sgview.h \
    $$PWD/sgmaterial.h \
    $$PWD/sggeometry.h \
    $$PWD/geometry/sgrectanglegeometry.h \
    $$PWD/geometry/sgcirculargeometry.h \
    $$PWD/material/sgcolormaterial.h \
    $$PWD/material/sgtexturematerial.h \
    $$PWD/material/sgglyphmaterial.h \
    $$PWD/node/sgrectanglenode.h \
    $$PWD/node/sgcircularnode.h \
    $$PWD/node/sgtextnode.h \
    $$PWD/util/sgtexture.h \
    $$PWD/util/sgimagetexture.h \
    $$PWD/util/sgglyphtexture.h \
    $$PWD/viewinteraction.h
