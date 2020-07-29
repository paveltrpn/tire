
CC       := clang++-10
CFLAGS   := --std=c++17
LIBS 	 := 
RM       := rm -f

SRCS := sdl2_canvas.cpp canvas.cpp
OBJS := $(SRCS:.cpp=.o)

.PHONY: clean all

$(info COMMON MAKEFILE)

all: compile link

compile: sdl2_canvas.o canvas.o

link: 
	$(CC) $(OBJS) -o main -lstdc++ -ljpeg -ltga -lSDL2
	
sdl2_canvas.o: sdl2_canvas.cpp
	$(CC) $(CFLAGS) -I/usr/include/SDL2 -c sdl2_canvas.cpp

canvas.o: canvas.cpp
	$(CC) $(CFLAGS) -I/usr/include/SDL2 -c canvas.cpp

clean:
	rm -f *.o main 