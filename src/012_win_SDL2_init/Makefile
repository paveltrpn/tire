
PKG_SDL_INCDIR := ../extern/SDL/include
PKG_SDL_LIBDIR := ../extern/SDL/build

PKG_LIBJPEG_INCDIR := ../extern/libjpeg-cmake
PKG_LIBJPEG_LIBDIR := ../extern/libjpeg-cmake/build

CC       := g++
CFLAGS   := -Wall -Wextra -Wpedantic -Wno-unused-parameter -std=c++20
LFLAGS 	 := -lstdc++ -llibjpeg -lSDL2

INCDIR	 =	$(PKG_SDL_INCDIR) \
	    	$(PKG_LIBJPEG_INCDIR)
	
LIBDIR	 =	$(PKG_SDL_LIBDIR) \
			$(PKG_LIBJPEG_LIBDIR)

BUILD_PREFIX = build

VPATH := .

SRCS = win_SDL2_init.cpp \
	   bmp.cpp \
	   tga.cpp \
	   jpeg.cpp
	  
OBJS := $(patsubst %.cpp,$(BUILD_PREFIX)/%.o,$(SRCS))

.PHONY: clean all

all: main

$(BUILD_PREFIX)/win_SDL2_init.o: win_SDL2_init.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/bmp.o: bmp.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/tga.o: tga.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/jpeg.o: jpeg.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

main: $(OBJS)
	$(CC) $(OBJS) $(addprefix -L,$(LIBDIR)) -o main $(LFLAGS)

clean:
	rm -f $(BUILD_PREFIX)/*.o main.exe