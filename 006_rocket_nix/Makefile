################################
# Linux-специфичный Makefile #
################################
CC       = g++
CFLAGS   = -Wall -std=c++17 
LFLAGS 	 = -lstdc++ -lGL -lGLU -lGLEW -lglfw -ljpeg -ltga
SOURCE_PREFIX = . src src/algebra
BUILD_PREFIX = build

VPATH := $(SOURCE_PREFIX)

SRCS = main.cpp \
	   mtrx4.cpp \
	   vec3.cpp \
	   vec4.cpp \
	   qtnn.cpp \
	   plane.cpp \
	   bitmap.cpp \
	   bitmap_text.cpp

OBJS := $(patsubst %.cpp,$(BUILD_PREFIX)/%.o,$(SRCS))

.PHONY: clean all

all: main

$(BUILD_PREFIX)/%.o: %.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(source_dirs)) -c $< -o $@

main: $(OBJS)
	$(CC) $^ $(LIBDIR) -o $@ $(LFLAGS)

clean:
	rm -f $(BUILD_PREFIX)/*.o main
