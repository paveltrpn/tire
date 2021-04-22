
CC       := g++
CFLAGS   := --std=c++17

INCDIR	 = E:/devtools/usr/local/include
LIBDIR	 = E:/devtools/usr/local/lib

LFLAGS 	 = -lstdc++ -llibjpeg -lSDL2

SOURCE_PREFIX = ../img

BUILD_PREFIX = build

VPATH := $(SOURCE_PREFIX)

SRCS = win_SDL2_init.cpp \
	   bmp.cpp \
	   tga.cpp \
	   jpeg.cpp
	  
OBJS := $(patsubst %.cpp,$(BUILD_PREFIX)/%.o,$(SRCS))

.PHONY: clean all

all: main

$(BUILD_PREFIX)/%.o: %.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

main: $(OBJS)
	$(CC) $^ -L$(LIBDIR) -o $@ $(LFLAGS)

clean:
	rm -f $(BUILD_PREFIX)/*.o main