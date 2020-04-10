.PHONY : mingw sgglfw macos linux undefined

CFLAGS := -g -std=c++11 -Wall -I../../include -I../example-scene/
LDFLAGS := -L../../src -lsgcore

SRC := \
    ../example-scene/scene.cpp \
    ../example-scene/scenefantacy.cpp \
    ../example-scene/scenevideo.cpp \
    view.cpp \
    main.cpp


UNAME=$(shell uname)
SYS=$(if $(filter Linux%,$(UNAME)),linux,\
	    $(if $(filter MINGW%,$(UNAME)),mingw,\
	        $(if $(filter Darwin%,$(UNAME)),macos,\
	            undefined\
)))

all: $(SYS)

undefined:
	@echo "I can't guess your platform, please do 'make PLATFORM' where PLATFORM is one of these:"
	@echo "      linux mingw macos"


mingw : TARGET := sgglfw.exe
mingw : CFLAGS += -I/usr/include -I/usr/local/include -I.
mingw : LDFLAGS += -L/usr/bin -lglfw3 -L/usr/local/bin
linux : TARGET := sgglfw.out
linux : CFLAGS += -I/usr/include -I/usr/local/include -I.
linux : LDFLAGS += -L/usr/bin -lglfw -pthread -L/usr/local/bin
macos : TARGET := sgglfw
macos : CFLAGS += -I/usr/include -I/usr/local/include -I3rdparty/include
macos : LDFLAGS += -L/usr/bin -L/usr/local/bin -L3rdparty/lib64 -lglfw -framework Cocoa -framework IOKit -framework CoreVideo


macos : $(SRC) sgglfw
mingw : $(SRC) sgglfw
linux : $(SRC) sgglfw

sgglfw :
	g++ $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

.PHONY: clean
clean:
	-rm -f $(TARGET)
