
PKG_LIBJPEG_INCDIR := ../extern/libjpeg-cmake
PKG_LIBJPEG_LIBDIR := ../extern/libjpeg-cmake/build

PKG_LIBTGA_INCDIR := ../extern/libtga/include
PKG_LIBTGA_LIBDIR := ../extern/libtga/build/src

CC       = g++
CFLAGS   = -Wall -Wextra -Wpedantic -Wno-unused-parameter -std=c++20
LFLAGS 	 := -lstdc++ -ltga -llibjpeg

INCDIR	 =	$(PKG_LIBJPEG_INCDIR) \
			$(PKG_LIBTGA_INCDIR)
	
LIBDIR	 =	$(PKG_LIBJPEG_LIBDIR) \
			$(PKG_LIBTGA_LIBDIR)

BUILD_PREFIX = build

SRCS := test.cpp canvas.cpp
OBJS := $(patsubst %.cpp,$(BUILD_PREFIX)/%.o,$(SRCS))

.PHONY: clean all

all: main

$(BUILD_PREFIX)/test.o: test.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/canvas.o: canvas.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

main: $(OBJS)
	$(CC) $(OBJS) $(addprefix -L,$(LIBDIR)) -o main $(LFLAGS)

clean:
	rm -f $(BUILD_PREFIX)/*.o main.exe