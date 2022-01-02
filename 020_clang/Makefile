PKG_GLFW_INCDIR = ../extern/glfw/include
PKG_GLFW_LIBDIR = ../extern/glfw/build/src

PKG_GLEW_INCDIR = ../extern/glew-2.1.0/include
PKG_GLEW_SRCDIR = ../extern/glew-2.1.0/src

CC       = gcc
CFLAGS   = -Wall 
LFLAGS 	 = -lm -lglfw3

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux)
	LFLAGS += -lGL -lGLU -lpthread -ldl
endif

ifeq ($(OS), Windows_NT)
	LFLAGS += -lopengl32 -lglu32 -lgdi32
endif

INCDIR	 = 	$(PKG_GLFW_INCDIR) \
			$(PKG_GLEW_INCDIR)

LIBDIR	 = $(PKG_GLFW_LIBDIR)

VPATH = src ../algebra_c $(PKG_GLEW_SRCDIR)

SRCS = glew.c common.c vec2.c vec3.c vec4.c mtrx3.c mtrx4.c main.c service.c

OBJS = $(patsubst %.c, build/%.o,$(SRCS))

#OBJS := $(SRCS:.cpp=.o)

.PHONY: clean all

all: $(OBJS) main

build/glew.o: glew.c
	$(CC) -w $(addprefix -I,$(INCDIR)) -c $< -o $@

build/common.o: common.c
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

build/vec2.o: vec2.c
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

build/vec3.o: vec3.c
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

build/vec4.o: vec4.c
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

build/mtrx3.o: mtrx3.c
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

build/mtrx4.o: mtrx4.c
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

build/main.o: main.c
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

build/service.o: service.c
	$(CC) -w $(addprefix -I,$(INCDIR)) -c $< -o $@

main: $(OBJS)
	$(CC) $(OBJS) $(addprefix -L,$(LIBDIR)) -o main $(LFLAGS)

clean:
	rm -f build/*.o main