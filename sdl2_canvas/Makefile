
CC       := g++
CFLAGS   := --std=c++17

INCDIR	 = E:/devtools/usr/local/include
LIBDIR	 = E:/devtools/usr/local/lib

SRCS := sdl2_canvas.cpp canvas.cpp
OBJS := $(SRCS:.cpp=.o)

.PHONY: clean all

$(info COMMON MAKEFILE)

all: compile link

compile: sdl2_canvas.o canvas.o

link: 
	$(CC) $(OBJS) -L$(LIBDIR) -o main -lstdc++ -llibjpeg -ltga -lSDL2
	
sdl2_canvas.o: sdl2_canvas.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) -c sdl2_canvas.cpp

canvas.o: canvas.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) -c canvas.cpp

clean:
	rm -f *.o main 