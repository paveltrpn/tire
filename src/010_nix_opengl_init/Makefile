
CC       := g++
CFLAGS   := -Wall -std=c++17
LFLAGS 	 := -lstdc++ -lSDL2
RM       := rm -f

INCDIR 	:=
LIBDIR 	:=
algebra_cpp	:=

LFLAGS += -lGL -lGLU -lGLX  -lglut -lX11 -lGLEW -lglfw
INCDIR += -I $(HOME)/code/algebra2_cpp/
algebra2_cpp += $(HOME)/code/algebra2_cpp/


SRCS := main.cpp \
		mainApp.cpp \
		mesh.cpp \
		$(algebra2_cpp)mtrx2.cpp \
		$(algebra2_cpp)mtrx3.cpp \
		$(algebra2_cpp)mtrx4.cpp \
		$(algebra2_cpp)vec2.cpp \
		$(algebra2_cpp)vec3.cpp \
		$(algebra2_cpp)vec4.cpp \
		$(algebra2_cpp)qtnn.cpp \
		$(algebra2_cpp)plane.cpp

OBJS := main.o \
		mainApp.o \
		mesh.o \
		$(algebra2_cpp)mtrx2.o \
		$(algebra2_cpp)mtrx3.o \
		$(algebra2_cpp)mtrx4.o \
		$(algebra2_cpp)vec2.o \
		$(algebra2_cpp)vec3.o \
		$(algebra2_cpp)vec4.o \
		$(algebra2_cpp)qtnn.o \
		$(algebra2_cpp)plane.o

#OBJS := $(SRCS:.cpp=.o)

all: main

main: $(OBJS)
	$(CC) $(OBJS) $(LIBDIR) -o main $(LFLAGS)

main.o: main.cpp
	$(CC) $(CFLAGS) $(INCDIR) -c main.cpp

mainApp.o: mainApp.cpp
	$(CC) $(CFLAGS) $(INCDIR) -c mainApp.cpp

mesh.o: mesh.cpp
	$(CC) $(CFLAGS) $(INCDIR) -c mesh.cpp

mtrx2.o: mtrx2.cpp
	$(CC) $(CFLAGS) $(INCDIR) -c $(algebra2_cpp)mtrx2.cpp

mtrx3.o: mtrx3.cpp
	$(CC) $(CFLAGS) $(INCDIR) -c $(algebra2_cpp)mtrx3.cpp

mtrx4.o: mtrx4.cpp
	$(CC) $(CFLAGS) $(INCDIR) -c $(algebra2_cpp)mtrx4.cpp

vec2.o: vec2.cpp
	$(CC) $(CFLAGS) $(INCDIR) -c $(algebra2_cpp)vec2.cpp

vec3.o: vec3.cpp
	$(CC) $(CFLAGS) $(INCDIR) -c $(algebra2_cpp)vec3.cpp

vec4.o: vec4.cpp
	$(CC) $(CFLAGS) $(INCDIR) -c $(algebra2_cpp)vec4.cpp

qtnn.o: qtnn.cpp
	$(CC) $(CFLAGS) $(INCDIR) -c $(algebra2_cpp)qtnn.cpp

plane.o: plane.cpp
	$(CC) $(CFLAGS) $(INCDIR) -c $(algebra2_cpp)plane.cpp

clean:
	$(RM) *.o
	$(RM) main
