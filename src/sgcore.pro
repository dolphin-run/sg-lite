TEMPLATE = lib
CONFIG += c++11 plugin
CONFIG -= app_bundle
CONFIG -= qt flat

#Use GLAD/GLEW OpenGL Library, comments this line will switch to glew
#CONFIG += glad

contains(QT_ARCH, i386) {
    PLATFORM = x86
} else {
    PLATFORM = x64
    ARCH_SUFFIX = 64
}

CONFIG(debug, debug|release) {
    BUILD_SUFFIX = d
    CONFIG += console
}

DEFINES += SG_BUILDING_PLUGIN
DEFINES += SG_GL_GLAD
DEFINES += SG_VIEW_VERTICAL_MIRROR

TARGET = sgcore$${BUILD_SUFFIX}
DESTDIR = ../lib/$${PLATFORM}

INCLUDEPATH += 3rdparty/include

glad {
    DEFINES += SG_GL_GLAD
} else {
    LIBS += -L3rdparty/lib$${ARCH_SUFFIX} -lglew32 -lopengl32
}

SOURCES += \
    sgglwrapper.cpp \
    sgnode.cpp \
    sgstruct.cpp \
    sgrenderer.cpp \
    sgscene.cpp \
    sgview.cpp \
    sgviewcamera.cpp \
    sgmaterial.cpp \
    sggeometry.cpp \
    geometry/sgrectanglegeometry.cpp \
    geometry/sgcirculargeometry.cpp \
    material/sgcolormaterial.cpp \
    material/sgtexturematerial.cpp \
    material/sgglyphmaterial.cpp \
    material/sgyuvmaterial.cpp \
    material/sgdynimagematerial.cpp \
    node/sgrectanglenode.cpp \
    node/sgcircularnode.cpp \
    node/sgtextnode.cpp \
    util/sgglyph.cpp \
    util/glyphloader.cpp \
    util/glshaderprogram.cpp \
    sgtexture.cpp \
    texture/sggridimagetexture.cpp\
    texture/sgyuvtexture.cpp\
    texture/sgdynimagetexture.cpp

HEADERS += \
    sgglobal.h \
    sgglwrapper.h \
    sgnode.h \
    sgnode_p.h \
    sgrootnode_p.h \
    sgstruct.h \
    sgrenderer.h \
    sgscene.h \
    sgscene_p.h \
    sgview.h \
    sgviewcamera.h \
    sgviewcamera_p.h \
    sgmaterial.h \
    sggeometry.h \
    geometry/sgrectanglegeometry.h \
    geometry/sgcirculargeometry.h \
    material/sgcolormaterial.h \
    material/sgtexturematerial.h \
    material/sgglyphmaterial.h \
    material/sgyuvmaterial.h \
    material/sgdynimagematerial.h \
    node/sgrectanglenode.h \
    node/sgcircularnode.h \
    node/sgtextnode.h \
    node/sgtextnode_p.h \
    util/sgglyph.h \
    util/glyphloader.h \
    util/glshaderprogram.h \
    sgtexture.h \
    texture/sggridimagetexture.h\
    texture/sgyuvtexture.h \
    texture/sgdynimagetexture.h

#INSTALL Define
target.files += \
    sgglobal.h \
    sgstruct.h \
    sgnode.h \
    sgview.h \
    sgviewcamera.h \
    sgscene.h
target.path += ../include
INSTALLS += target

targetnode.files += \
    node/sgrectanglenode.h \
    node/sgcircularnode.h \
    node/sgtextnode.h
targetnode.path += ../include/node
INSTALLS += targetnode
