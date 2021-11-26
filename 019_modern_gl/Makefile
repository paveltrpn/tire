PKG_FMT_INCDIR := ../extern/fmt/include
PKG_FMT_LIBDIR := ../extern/fmt/build

PKG_GLFW_INCDIR := ../extern/glfw/include
PKG_GLFW_LIBDIR := ../extern/glfw/build/src

PKG_GLEW_INCDIR := ../extern/glew-2.1.0/include
PKG_GLEW_SRCDIR := ../extern/glew-2.1.0/src

CC       = g++

CFLAGS   = -Wall -Wextra -Wpedantic -Wno-unused-parameter -std=c++20
LFLAGS 	 = -lstdc++ -lfmt -lopengl32 -lglu32 -lglfw3 -lgdi32

INCDIR	 := $(PKG_FMT_INCDIR) \
			$(PKG_GLFW_INCDIR) \
			$(PKG_GLEW_INCDIR)

LIBDIR	 := $(PKG_FMT_LIBDIR) \
			$(PKG_GLFW_LIBDIR)
			
BUILD_PREFIX = build

VPATH := src $(PKG_GLEW_SRCDIR)

SRCS := glew.cpp \
		main.cpp \
		service.cpp \
		shader.cpp

OBJS := $(patsubst %.cpp,$(BUILD_PREFIX)/%.o,$(SRCS))

#OBJS := $(SRCS:.cpp=.o)

.PHONY: clean all

all: $(OBJS) main

$(BUILD_PREFIX)/glew.o: glew.c
	$(CC) -Wno-attributes -Wno-cast-function-type $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/main.o: main.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/service.o: service.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

$(BUILD_PREFIX)/shader.o: shader.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

main: $(OBJS)
	$(CC) $(OBJS) $(addprefix -L,$(LIBDIR)) -o main $(LFLAGS)

clean:
	rm -f build/*.o main