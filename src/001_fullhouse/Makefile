CC       = g++
INCDIR	 := E:/devtools/usr/local/include
LIBDIR	 := E:/devtools/usr/local/lib
CFLAGS   = -Wall -Wextra -Wpedantic -Wno-unused-parameter -std=c++20 -O2
LFLAGS 	 = -lstdc++ -lopengl32 -lglu32 -lglfw3 -lgdi32 -lglew32 -ltga -llibjpeg -lfmt

SOURCE_PREFIX = src ../algebra ../img ../timer
BUILD_PREFIX = build

VPATH := $(SOURCE_PREFIX)

SRCS := main.cpp \
		mainApp.cpp \
		mesh.cpp \
		mtrx2.cpp \
		mtrx3.cpp \
		mtrx4.cpp \
		vec2.cpp \
		vec3.cpp \
		vec4.cpp \
		qtnn.cpp \
		plane.cpp \
		bitmap.cpp \
		bitmap_text.cpp \
		camera.cpp

OBJS := $(patsubst %.cpp,$(BUILD_PREFIX)/%.o,$(SRCS))

#OBJS := $(SRCS:.cpp=.o)

.PHONY: clean all

all: $(OBJS) main

$(BUILD_PREFIX)/main.o: main.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

$(BUILD_PREFIX)/mainApp.o: mainApp.cpp mainApp.h
	$(CC) $(CFLAGS) -I$(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

$(BUILD_PREFIX)/mesh.o: mesh.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

$(BUILD_PREFIX)/mtrx2.o: mtrx2.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

$(BUILD_PREFIX)/mtrx3.o: mtrx3.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

$(BUILD_PREFIX)/mtrx4.o: mtrx4.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

$(BUILD_PREFIX)/vec2.o: vec2.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

$(BUILD_PREFIX)/vec3.o: vec3.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

$(BUILD_PREFIX)/vec4.o: vec4.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

$(BUILD_PREFIX)/qtnn.o: qtnn.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

$(BUILD_PREFIX)/plane.o: plane.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

$(BUILD_PREFIX)/bitmap.o: bitmap.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

$(BUILD_PREFIX)/bitmap_text.o: bitmap_text.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

$(BUILD_PREFIX)/camera.o: camera.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

main: $(OBJS)
	$(CC) $(OBJS) -L$(LIBDIR) -o main $(LFLAGS)

clean:
	rm -f build/*.o main
