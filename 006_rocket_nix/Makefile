################################
# Linux-специфичный Makefile #
################################
CC       = g++-10
CFLAGS   = -Wall -std=c++20 
LFLAGS 	 = -lstdc++ -lGL -lGLU -lGLEW -lglfw -ljpeg -ltga

SRCS = src/main.cpp \
		src/mtrx4.cpp \
		src/vec3.cpp \
		src/vec4.cpp \
		src/qtnn.cpp \
		src/plane.cpp \
		src/bitmap.cpp \
		src/bitmap_text.cpp
		

OBJS = main.o \
		mtrx4.o \
		vec3.o \
		vec4.o \
		qtnn.o \
		plane.o \
		bitmap.o \
		bitmap_text.o

#OBJS := $(SRCS:.cpp=.o)

.PHONY: clean all

all: $(OBJS) main

main.o: src/main.cpp
	$(CC) $(CFLAGS) $(INCDIR) -c src/main.cpp

mtrx4.o: src/mtrx4.cpp
	$(CC) $(CFLAGS) $(INCDIR) -c src/mtrx4.cpp

vec3.o: src/vec3.cpp
	$(CC) $(CFLAGS) $(INCDIR) -c src/vec3.cpp

vec4.o: src/vec4.cpp
	$(CC) $(CFLAGS) $(INCDIR) -c src/vec4.cpp

qtnn.o: src/qtnn.cpp
	$(CC) $(CFLAGS) $(INCDIR) -c src/qtnn.cpp

plane.o: src/plane.cpp
	$(CC) $(CFLAGS) $(INCDIR) -c src/plane.cpp

bitmap.o: src/bitmap.cpp
	$(CC) $(CFLAGS) $(INCDIR) -c src/bitmap.cpp

bitmap_text.o: src/bitmap_text.cpp
	$(CC) $(CFLAGS) $(INCDIR) -c src/bitmap_text.cpp

main: $(OBJS)
	g++ $(OBJS) $(LIBDIR) -o main $(LFLAGS)

clean:
	rm -f *.o main
