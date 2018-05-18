.PHONY : mingw sgcore macos linux undefined

CFLAGS := -g -shared -std=c++11 -Wall -DSG_BUILDING_PLUGIN -I3rdparty/include
#comment this to use GLEW
CFLAGS += -DSG_GL_GLAD

LDFLAGS :=

SRC := \
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


UNAME=$(shell uname)
SYS=$(if $(filter Linux%,$(UNAME)),linux,\
	    $(if $(filter MINGW%,$(UNAME)),mingw,\
	        undefined\
))

all: $(SYS)

undefined:
	@echo "I can't guess your platform, please do 'make PLATFORM' where PLATFORM is one of these:"
	@echo "      linux mingw"


mingw : TARGET := sgcore.dll
mingw : CFLAGS += -I/usr/include -I/usr/local/include -I.
mingw : LDFLAGS += -L/usr/bin -lglew32 -lopengl32 -L/usr/local/bin
macos : TARGET := libsgcore.dylib
macos : CFLAGS += -I/usr/include -I/usr/local/include -I.
macos : LDFLAGS += -L/usr/bin -L/usr/local/bin  -framework OpenGL


macos : $(SRC) sgcore
mingw : $(SRC) sgcore

sgcore :
	g++ $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

.PHONY: clean
clean:
	-rm -f sgcore.dll
	-rm -f libsgcore.dylib

.PHONY: install
install:
	-mkdir -p ../include
	-mkdir -p ../include/node
	-mkdir -p ../lib

	-cp -f \
    sgglobal.h \
    sgstruct.h \
    sgnode.h \
    sgview.h \
    sgviewcamera.h \
    sgscene.h \
    ../include

	-cp -f \
    node/sgrectanglenode.h \
    node/sgcircularnode.h \
    node/sgtextnode.h \
    ../include/node
    
	-cp -f \
    sgcore.dll ../lib