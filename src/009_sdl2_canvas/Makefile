PKG_SDL_INCDIR := ../extern/SDL/include
PKG_SDL_LIBDIR := ../extern/SDL/build

PKG_LIBJPEG_INCDIR := ../extern/libjpeg-cmake
PKG_LIBJPEG_LIBDIR := ../extern/libjpeg-cmake/build

PKG_LIBTGA_INCDIR := ../extern/libtga/include
PKG_LIBTGA_LIBDIR := ../extern/libtga/build/src

CC       := g++
CFLAGS   := -Wall -Wextra -Wpedantic -Wno-unused-parameter -std=c++20
LFLAGS 	 := -lstdc++ -lSDL2 -ltga -llibjpeg

INCDIR	 =	$(PKG_SDL_INCDIR) \
	    	$(PKG_LIBJPEG_INCDIR) \
			$(PKG_LIBTGA_INCDIR)
	
LIBDIR	 =	$(PKG_SDL_LIBDIR) \
			$(PKG_LIBJPEG_LIBDIR) \
			$(PKG_LIBTGA_LIBDIR)

BUILD_PREFIX = build

SRCS := sdl2_canvas.cpp canvas.cpp

OBJS := $(patsubst %.cpp,$(BUILD_PREFIX)/%.o,$(SRCS))

.PHONY: clean all

all: main
	
$(BUILD_PREFIX)/sdl2_canvas.o: sdl2_canvas.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/canvas.o: canvas.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

main: $(OBJS)
	$(CC) $(OBJS) $(addprefix -L,$(LIBDIR)) -o main $(LFLAGS)

clean:
	rm -f $(BUILD_PREFIX)/*.o main.exe