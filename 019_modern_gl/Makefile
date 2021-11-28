PKG_FMT_INCDIR := ../extern/fmt/include
PKG_FMT_LIBDIR := ../extern/fmt/build

PKG_GLFW_INCDIR := ../extern/glfw/include
PKG_GLFW_LIBDIR := ../extern/glfw/build/src

PKG_GLEW_INCDIR := ../extern/glew-2.1.0/include
PKG_GLEW_SRCDIR := ../extern/glew-2.1.0/src

PKG_IMGUI_INCDIR := ../extern/imgui
PKG_IMGUI_SRCDIR := ../extern/imgui ../extern/imgui/backends

PKG_LIBJPEG_INCDIR := ../extern/libjpeg-cmake/include
PKG_LIBJPEG_LIBDIR := ../extern/libjpeg-cmake/build

PKG_LIBTGA_INCDIR := ../extern/libtga/include
PKG_LIBTGA_LIBDIR := ../extern/libtga/build/src

CC       = g++
CFLAGS   = -Wall -Wextra -Wpedantic -Wno-unused-parameter -std=c++20
LFLAGS 	 = -lstdc++ -lfmt -lopengl32 -lglu32 -lglfw3 -lgdi32

INCDIR	 := $(PKG_FMT_INCDIR) \
			$(PKG_GLFW_INCDIR) \
			$(PKG_GLEW_INCDIR) \
			$(PKG_IMGUI_INCDIR)

LIBDIR	 := $(PKG_FMT_LIBDIR) \
			$(PKG_GLFW_LIBDIR)
			
BUILD_PREFIX = build

VPATH := src $(PKG_GLEW_SRCDIR) $(PKG_IMGUI_SRCDIR)

SRCS := glew.cpp \
		main.cpp \
		service.cpp \
		shader.cpp \
		imgui.cpp \
		imgui_draw.cpp \
		imgui_tables.cpp \
		imgui_widgets.cpp \
		imgui_impl_glfw.cpp \
		imgui_impl_opengl3.cpp

OBJS := $(patsubst %.cpp,$(BUILD_PREFIX)/%.o,$(SRCS))

#OBJS := $(SRCS:.cpp=.o)

.PHONY: clean all

all: $(OBJS) main

$(BUILD_PREFIX)/glew.o: glew.c
	$(CC) -w $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/main.o: main.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/service.o: service.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/shader.o: shader.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/imgui.o: imgui.cpp
	$(CC) -w $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/imgui_draw.o: imgui_draw.cpp
	$(CC) -w $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/imgui_tables.o: imgui_tables.cpp
	$(CC) -w $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/imgui_widgets.o: imgui_widgets.cpp
	$(CC) -w $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/imgui_impl_glfw.o: imgui_impl_glfw.cpp
	$(CC) -w $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/imgui_impl_opengl3.o: imgui_impl_opengl3.cpp
	$(CC) -w $(addprefix -I,$(INCDIR)) -c $< -o $@

main: $(OBJS)
	$(CC) $(OBJS) $(addprefix -L,$(LIBDIR)) -o main $(LFLAGS)

clean:
	rm -f build/*.o main