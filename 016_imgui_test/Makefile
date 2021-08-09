CC       = g++
INCDIR	 := -IE:/devtools/usr/local/include
LIBDIR	 := -LE:/devtools/usr/local/lib
CFLAGS   = -Wall -Wextra -Wpedantic -Wno-unused-parameter -std=c++20
LFLAGS 	 = -lstdc++ -lopengl32 -lglu32 -lglfw3 -lgdi32 -lglew32 -lfmt 

SOURCE_PREFIX = src ../3d_party/imgui ../3d_party/imgui/backends ../3d_party/ 
BUILD_PREFIX = build

VPATH := $(SOURCE_PREFIX)

SRCS := main.cpp \
		apputils.cpp \
		camera.cpp \
		timing.cpp \
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
	$(CC) $(CFLAGS) $(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

$(BUILD_PREFIX)/apputils.o: apputils.cpp
	$(CC) $(CFLAGS) $(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

$(BUILD_PREFIX)/camera.o: camera.cpp
	$(CC) $(CFLAGS) $(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

$(BUILD_PREFIX)/timing.o: timing.cpp
	$(CC) $(CFLAGS) $(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

$(BUILD_PREFIX)/imgui.o: imgui.cpp
	$(CC) $(CFLAGS) $(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

$(BUILD_PREFIX)/imgui_demo.o: imgui_demo.cpp
	$(CC) $(CFLAGS) $(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

$(BUILD_PREFIX)/imgui_draw.o: imgui_draw.cpp
	$(CC) $(CFLAGS) $(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

$(BUILD_PREFIX)/imgui_tables.o: imgui_tables.cpp
	$(CC) $(CFLAGS) $(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

$(BUILD_PREFIX)/imgui_widgets.o: imgui_widgets.cpp
	$(CC) $(CFLAGS) $(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

$(BUILD_PREFIX)/imgui_impl_glfw.o: imgui_impl_glfw.cpp
	$(CC) $(CFLAGS) $(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

$(BUILD_PREFIX)/imgui_impl_opengl2.o: imgui_impl_opengl2.cpp
	$(CC) $(CFLAGS) $(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

main: $(OBJS)
	$(CC) $(OBJS) $(LIBDIR) -o main $(LFLAGS)

clean:
	rm -f build/*.o main