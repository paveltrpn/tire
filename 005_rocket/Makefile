################################
# Windows-специфичный Makefile #
################################
CC       = g++
INCDIR	 = E:/devtools/usr/local/include
LIBDIR	 = E:/devtools/usr/local/lib
CFLAGS   = -Wall -std=c++20
LFLAGS 	 = -lstdc++ -lopengl32 -lglfw3 -lgdi32 -lglew32 -ltga -llibjpeg

SOURCE_PREFIX = src ../algebra ../img
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
	$(CC) $(CFLAGS) -I$(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

main: $(OBJS)
	$(CC) $^ -L$(LIBDIR) -o $@ $(LFLAGS)

clean:
	rm -f $(BUILD_PREFIX)/*.o main
