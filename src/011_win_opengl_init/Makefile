
CC       := g++
CFLAGS   := --std=c++17

INCDIR	 = E:/devtools/usr/local/include
LIBDIR	 = E:/devtools/usr/local/lib

LFLAGS 	 = -lstdc++ -lopengl32 -lglu32 -lglfw3 -lgdi32 -lglew32

SOURCE_PREFIX = ../algebra

BUILD_PREFIX = build

VPATH := $(SOURCE_PREFIX)

SRCS = win_opengl_init.cpp \
	   mtrx4.cpp \
	   mtrx3.cpp \
	   vec3.cpp \
	   vec4.cpp \
	   qtnn.cpp \
	   cmplx.cpp \
	   plane.cpp

OBJS := $(patsubst %.cpp,$(BUILD_PREFIX)/%.o,$(SRCS))

.PHONY: clean all

all: main

$(BUILD_PREFIX)/%.o: %.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

main: $(OBJS)
	$(CC) $^ -L$(LIBDIR) -o $@ $(LFLAGS)

clean:
	rm -f $(BUILD_PREFIX)/*.o main