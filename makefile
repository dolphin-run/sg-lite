.PHONY : mingw ej2d linux undefined

CFLAGS := -g -std=c++11 -Wall -Isrc -Isrc/3rdparty/include
LDFLAGS := 

SRC := \
    src/sgnode.cpp \
    src/sgstruct.cpp \
    src/sgrenderer.cpp \
    src/sgscene.cpp \
    src/sgview.cpp \
    src/sgmaterial.cpp \
    src/sggeometry.cpp \
    src/geometry/sgrectanglegeometry.cpp \
    src/geometry/sgcirculargeometry.cpp \
    src/material/sgcolormaterial.cpp \
    src/material/sgtexturematerial.cpp \
    src/material/sgglyphmaterial.cpp \
    src/node/sgrectanglenode.cpp \
    src/node/sgcircularnode.cpp \
    src/node/sgtextnode.cpp \
    src/util/sgglyph.cpp \
    src/util/glyphloader.cpp \
    src/util/glshaderprogram.cpp \
    src/sgtexture.cpp \
    src/texture/sggridimagetexture.cpp\
    src/3rdparty/glad/glad.c


UNAME=$(shell uname)
SYS=$(if $(filter Linux%,$(UNAME)),linux,\
	    $(if $(filter MINGW%,$(UNAME)),mingw,\
	        undefined\
))

all: $(SYS)

undefined:
	@echo "I can't guess your platform, please do 'make PLATFORM' where PLATFORM is one of these:"
	@echo "      linux mingw"


mingw : TARGET := ej2d.exe
mingw : CFLAGS += -I/usr/include -I/usr/local/include
mingw : LDFLAGS += -L/usr/bin -lgdi32 -lglfw3 -lfreetype -lopengl32 -L/usr/local/bin
mingw : SRC += mingw/view.cpp mingw/scene.cpp mingw/main.cpp

mingw : $(SRC) ej2d

ej2d :
	g++ $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

.PHONY: clean
clean:
	-rm -f ej2d.exe
	-rm -f ej2d