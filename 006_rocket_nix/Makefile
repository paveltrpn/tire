################################
# Linux-специфичный Makefile #
################################
CC       = g++-10
CFLAGS   = -Wall -std=c++20 
LFLAGS 	 = -lstdc++ -lGL -lGLU -lGLEW -lglfw -ljpeg -ltga
SOURCE_PREFIX = src
BUILD_PREFIX = build

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

$(BUILD_PREFIX)/%.o: $(SOURCE_PREFIX)/%.cpp
	$(CC) $(CFLAGS) $(INCDIR) -c $< -o $@

main: $(OBJS)
	$(CC) $^ $(LIBDIR) -o $@ $(LFLAGS)

clean:
	rm -f $(BUILD_PREFIX)/*.o main
