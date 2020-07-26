
CC       := clang++-10
CFLAGS   := --std=c++17
LIBS 	 := 
RM       := rm -f

SRCS := test.cpp canvas.cpp
OBJS := $(SRCS:.cpp=.o)

.PHONY: clean all

$(info COMMON MAKEFILE)

all: compile link

compile: test.o canvas.o

link: 
	$(CC) $(OBJS) -o main -lstdc++ -ljpeg
	
test.o: test.cpp
	$(CC) $(CFLAGS) -c test.cpp

canvas.o: canvas.cpp
	$(CC) $(CFLAGS) -c canvas.cpp

clean:
	rm -f *.o main