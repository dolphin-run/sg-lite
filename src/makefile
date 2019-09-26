.PHONY : mingw sgcore macos linux undefined

CFLAGS := -shared -std=c++11 -Wl,--out-implib,libsgcore.a -DSG_BUILDING_PLUGIN -I3rdparty/include
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
mingw : LDFLAGS += -L/usr/bin -lglfw3 -lfreetype -lglew32 -lopengl32 -L/usr/local/bin
macos : TARGET := sgcore.dylib
macos : CFLAGS += -I/usr/include -I/usr/local/include -Imingw/3rdparty/include
macos : LDFLAGS += -L/usr/bin -lfreetype -L/usr/local/bin -Lmingw/3rdparty/lib -lglfw3  -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo


macos : $(SRC) sgcore
mingw : $(SRC) sgcore

sgcore :
	g++ $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

.PHONY: clean
clean:
	-rm -f sgcore.dll

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