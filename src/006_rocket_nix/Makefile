PKG_GLFW_INCDIR := ../extern/glfw/include
PKG_GLFW_LIBDIR := ../extern/glfw/build/src

PKG_GLEW_INCDIR := ../extern/glew-2.1.0/include
PKG_GLEW_SRCDIR := ../extern/glew-2.1.0/src

PKG_LIBJPEG_INCDIR := ../extern/libjpeg-cmake
PKG_LIBJPEG_LIBDIR := ../extern/libjpeg-cmake/build

PKG_LIBTGA_INCDIR := ../extern/libtga/include
PKG_LIBTGA_LIBDIR := ../extern/libtga/build/src

CC       = g++
CFLAGS   = -Wall -std=c++20 
LFLAGS 	 = -lstdc++ -lGL -lGLU -lglfw3 -llibjpeg -ltga -lpthread -ldl

INCDIR	 := $(PKG_GLFW_INCDIR) \
			$(PKG_GLEW_INCDIR) \
			$(PKG_LIBJPEG_INCDIR) \
			$(PKG_LIBTGA_INCDIR) \
			../algebra

LIBDIR	 := $(PKG_GLFW_LIBDIR) \
			$(PKG_LIBJPEG_LIBDIR) \
			$(PKG_LIBTGA_LIBDIR)
			
BUILD_PREFIX = build

VPATH := src ../algebra $(PKG_GLEW_SRCDIR)

SRCS = glew.cpp \
	   main.cpp \
	   mtrx4.cpp \
	   vec3.cpp \
	   vec4.cpp \
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

$(BUILD_PREFIX)/plane.o: plane.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/bitmap.o: bitmap.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/bitmap_text.o: bitmap_text.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

main: $(OBJS)
	$(CC) $(OBJS) $(addprefix -L,$(LIBDIR)) -o main $(LFLAGS)

clean:
	rm -f $(BUILD_PREFIX)/*.o main
