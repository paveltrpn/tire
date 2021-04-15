################################
# Linux-специфичный Makefile #
################################
CC       = g++-10
CFLAGS   = -Wall -std=c++20 
LFLAGS 	 = -lstdc++ -lGL -lGLU -lGLEW -lglfw -ljpeg -ltga
SOURCE_PREFIX = src ../algebra ../img ../timer
BUILD_PREFIX = build

VPATH := $(SOURCE_PREFIX)

SRCS = main.cpp \
	   mtrx4.cpp \
	   vec3.cpp \
	   vec4.cpp \
	   qtnn.cpp \
	   cmplx.cpp \
	   plane.cpp \
	   bitmap.cpp \
	   bitmap_text.cpp

OBJS := $(patsubst %.cpp,$(BUILD_PREFIX)/%.o,$(SRCS))

.PHONY: clean all

all: main

$(BUILD_PREFIX)/%.o: %.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

main: $(OBJS)
	$(CC) $^ $(LIBDIR) -o $@ $(LFLAGS)

clean:
	rm -f $(BUILD_PREFIX)/*.o main
