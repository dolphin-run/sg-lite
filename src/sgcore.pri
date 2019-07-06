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
    $$PWD/sgtexture.cpp \
    $$PWD/texture/sgimagetexture.cpp \
    $$PWD/texture/sgglyphtexture.cpp \
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
    $$PWD/sgtexture.h \
    $$PWD/texture/sgimagetexture.h \
    $$PWD/texture/sgglyphtexture.h \
    $$PWD/viewinteraction.h
