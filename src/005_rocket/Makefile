
PKG_FMT_INCDIR := ../extern/fmt/include
PKG_FMT_LIBDIR := ../extern/fmt/build

PKG_GLEW_INCDIR := ../extern/glew-2.1.0/include
PKG_GLEW_SRCDIR := ../extern/glew-2.1.0/src

PKG_GLFW_INCDIR := ../extern/glfw/include
PKG_GLFW_LIBDIR := ../extern/glfw/build/src

PKG_LIBTGA_INCDIR := ../extern/libtga/include
PKG_LIBTGA_LIBDIR := ../extern/libtga/build/src

PKG_LIBJPEG_INCDIR := ../extern/libjpeg-cmake
PKG_LIBJPEG_LIBDIR := ../extern/libjpeg-cmake/build

CC       = g++
CFLAGS   = -Wall -Wextra -Wpedantic -Wno-unused-parameter -std=c++20
LFLAGS 	 = -lstdc++ -lfmt -lglfw3 -ltga -llibjpeg

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux)
	LFLAGS += -lGL -lGLU -lpthread -ldl
endif

ifeq ($(OS), Windows_NT)
	LFLAGS += -lopengl32 -lglu32 -lgdi32
endif

INCDIR	 =	$(PKG_FMT_INCDIR) \
			$(PKG_GLFW_INCDIR) \
			$(PKG_GLEW_INCDIR) \
			$(PKG_LIBJPEG_INCDIR) \
			$(PKG_LIBTGA_INCDIR) \
			../algebra
	
LIBDIR	 =	$(PKG_FMT_LIBDIR) \
			$(PKG_GLFW_LIBDIR) \
			$(PKG_LIBJPEG_LIBDIR) \
			$(PKG_LIBTGA_LIBDIR)

SOURCE_PREFIX = src ../algebra $(PKG_GLEW_SRCDIR)

BUILD_PREFIX = build

VPATH := $(SOURCE_PREFIX)

SRCS = glew.cpp \
		main.cpp \
	   mtrx4.cpp \
	   vec3.cpp \
	   vec4.cpp \
	   qtnn.cpp \
	   plane.cpp \
	   bitmap.cpp \
	   bitmap_text.cpp

OBJS := $(patsubst %.cpp,$(BUILD_PREFIX)/%.o,$(SRCS))

.PHONY: clean all

all: $(OBJS) main

$(BUILD_PREFIX)/glew.o: glew.c
	$(CC) -w $(addprefix -I,$(INCDIR)) -c $< -o $@
	
$(BUILD_PREFIX)/main.o: main.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/mtrx4.o: mtrx4.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/vec3.o: vec3.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/vec4.o: vec4.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/qtnn.o: qtnn.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/plane.o: plane.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/bitmap.o: bitmap.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/bitmap_text.o: bitmap_text.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

main: $(OBJS)
	$(CC) $(OBJS) $(addprefix -L,$(LIBDIR)) -o main $(LFLAGS)

clean:
	rm -f build/*.o main.exe
