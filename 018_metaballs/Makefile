CC       = g++

CFLAGS   = -Wall -Wextra -Wpedantic -Wno-unused-parameter -std=c++20
LFLAGS 	 = -lstdc++ -lfmt -lopengl32 -lglu32 -lglfw3 -lgdi32 -lglew32 -ltga -llibjpeg

INCDIR	 := ../extern/fmt/include/ \
			../extern/libtga/include \
			../extern/libjpeg/include \
			../extern/glfw/include \
			../extern/glew/include \
			../extern/imgui \
			../extern/

LIBDIR	 := ../extern/fmt/lib/ \
			../extern/glfw/lib \
			../extern/glew/lib \
			../extern/libtga/lib \
			../extern/libjpeg/lib \
			../extern/

BUILD_PREFIX = build

VPATH := src ../extern/imgui ../extern/imgui/backends

SRCS := main.cpp \
		apputils.cpp \
		camera.cpp \
		bitmap.cpp \
		imgui.cpp \
		imgui_demo.cpp \
		imgui_draw.cpp \
		imgui_tables.cpp \
		imgui_widgets.cpp \
		imgui_impl_glfw.cpp \
		imgui_impl_opengl2.cpp

OBJS := $(patsubst %.cpp,$(BUILD_PREFIX)/%.o,$(SRCS))

#OBJS := $(SRCS:.cpp=.o)

.PHONY: clean all

all: $(OBJS) main

$(BUILD_PREFIX)/main.o: main.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/apputils.o: apputils.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/camera.o: camera.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/bitmap.o: bitmap.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/imgui.o: imgui.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/imgui_demo.o: imgui_demo.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/imgui_draw.o: imgui_draw.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/imgui_tables.o: imgui_tables.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/imgui_widgets.o: imgui_widgets.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/imgui_impl_glfw.o: imgui_impl_glfw.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/imgui_impl_opengl2.o: imgui_impl_opengl2.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

main: $(OBJS)
	$(CC) $(OBJS) $(addprefix -L,$(LIBDIR)) -o main $(LFLAGS)

clean:
	rm -f build/*.o main