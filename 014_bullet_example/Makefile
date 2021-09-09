CC       = g++

CFLAGS   = -Wall -Wextra -Wpedantic -Wno-unused-parameter -std=c++20
LFLAGS 	 = -lstdc++ -lfmt -lopengl32 -lglu32 -lglfw3 -lgdi32 -ltga -llibjpeg -lBulletSoftBody -lBulletDynamics -lBulletCollision -lLinearMath

INCDIR	 := ../extern/fmt/include/ \
			../extern/libtga/include \
			../extern/libjpeg/include \
			../extern/glfw/include \
			../extern/glew/include \
			../extern/bullet3/include/ \
			../extern/bullet3/include/bullet \
			../extern/

LIBDIR	 := ../extern/fmt/lib/ \
			../extern/glfw/lib \
			../extern/libtga/lib \
			../extern/libjpeg/lib \
			../extern/bullet3/lib/ \
			../extern/bullet3/lib/bullet \
			../extern/
			
BUILD_PREFIX = build

VPATH := src ../extern/glew/src

SRCS := glew.cpp \
		main.cpp \
		apputils.cpp \
		bitmap.cpp \
		mesh.cpp \
		matbase.cpp \
		camera.cpp \
		timing.cpp \
		screen_text.cpp \
		btDebugDraw.cpp

OBJS := $(patsubst %.cpp,$(BUILD_PREFIX)/%.o,$(SRCS))

#OBJS := $(SRCS:.cpp=.o)

.PHONY: clean all

all: $(OBJS) main

$(BUILD_PREFIX)/glew.o: glew.cpp
	$(CC) -Wno-attributes -Wno-cast-function-type $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/main.o: main.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/apputils.o: apputils.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/bitmap.o: bitmap.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/mesh.o: mesh.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/matbase.o: matbase.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/camera.o: camera.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/timing.o: timing.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/screen_text.o: screen_text.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/btDebugDraw.o: btDebugDraw.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

main: $(OBJS)
	$(CC) $(OBJS) $(addprefix -L,$(LIBDIR)) -o main $(LFLAGS)

clean:
	rm -f build/*.o main