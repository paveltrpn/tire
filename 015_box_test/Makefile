PKG_FMT_INCDIR := ../extern/fmt/include
PKG_FMT_LIBDIR := ../extern/fmt/build

PKG_GLFW_INCDIR := ../extern/glfw/include
PKG_GLFW_LIBDIR := ../extern/glfw/build/src

PKG_GLEW_INCDIR := ../extern/glew-2.1.0/include
PKG_GLEW_SRCDIR := ../extern/glew-2.1.0/src

PKG_IMGUI_INCDIR := ../extern/imgui
PKG_IMGUI_SRCDIR := ../extern/imgui ../extern/imgui/backends

PKG_LIBJPEG_INCDIR := ../extern/libjpeg-cmake
PKG_LIBJPEG_LIBDIR := ../extern/libjpeg-cmake/build

PKG_LIBTGA_INCDIR := ../extern/libtga/include
PKG_LIBTGA_LIBDIR := ../extern/libtga/build/src

CC       = g++

CFLAGS   = -Wall -Wextra -Wpedantic -Wno-unused-parameter -std=c++20
LFLAGS 	 = -lstdc++ -lfmt -lopengl32 -lglu32 -lglfw3 -lgdi32 -ltga -llibjpeg

INCDIR	 := $(PKG_FMT_INCDIR) \
			$(PKG_LIBTGA_INCDIR) \
			$(PKG_LIBJPEG_INCDIR) \
			$(PKG_GLFW_INCDIR) \
			$(PKG_GLEW_INCDIR) \
			$(PKG_IMGUI_INCDIR) \
			../algebra

LIBDIR	 := $(PKG_FMT_LIBDIR) \
			$(PKG_GLFW_LIBDIR) \
			$(PKG_LIBTGA_LIBDIR) \
			$(PKG_LIBJPEG_LIBDIR)
			
BUILD_PREFIX = build/

VPATH := src ../algebra $(PKG_GLEW_SRCDIR)

SRCS := glew.cpp \
		mtrx2.cpp \
		mtrx3.cpp \
		mtrx4.cpp \
		vec2.cpp \
		vec3.cpp \
		vec4.cpp \
		qtnn.cpp \
		plane.cpp \
		main.cpp \
		apputils.cpp \
		bitmap.cpp \
		mesh.cpp \
		matbase.cpp \
		camera.cpp \
		timing.cpp \
		screen_text.cpp

OBJS := $(patsubst %.cpp,$(BUILD_PREFIX)/%.o,$(SRCS))

#OBJS := $(SRCS:.cpp=.o)

.PHONY: clean all

all: $(OBJS) main

$(BUILD_PREFIX)/glew.o: glew.c
	$(CC) -w $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/mtrx2.o: mtrx2.cpp
	$(CC) $(CFLAGS)  $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/mtrx3.o: mtrx3.cpp
	$(CC) $(CFLAGS)  $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/mtrx4.o: mtrx4.cpp
	$(CC) $(CFLAGS)  $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/vec2.o: vec2.cpp
	$(CC) $(CFLAGS)  $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/vec3.o: vec3.cpp
	$(CC) $(CFLAGS)  $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/vec4.o: vec4.cpp
	$(CC) $(CFLAGS)  $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/qtnn.o: qtnn.cpp
	$(CC) $(CFLAGS)  $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/plane.o: plane.cpp
	$(CC) $(CFLAGS)  $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/main.o: main.cpp
	$(CC) $(CFLAGS)  $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/apputils.o: apputils.cpp
	$(CC) $(CFLAGS)  $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/bitmap.o: bitmap.cpp
	$(CC) $(CFLAGS)  $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/mesh.o: mesh.cpp
	$(CC) $(CFLAGS)  $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/matbase.o: matbase.cpp
	$(CC) $(CFLAGS)  $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/camera.o: camera.cpp
	$(CC) $(CFLAGS)  $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/timing.o: timing.cpp
	$(CC) $(CFLAGS)  $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/screen_text.o: screen_text.cpp
	$(CC) $(CFLAGS)  $(addprefix -I,$(INCDIR)) -c $< -o $@

main: $(OBJS)
	$(CC) $(OBJS) $(addprefix -L,$(LIBDIR)) -o main $(LFLAGS)

clean:
	rm -f build/*.o main
