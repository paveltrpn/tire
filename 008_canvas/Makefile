
CC       := g++
CFLAGS   := --std=c++17
LIBS 	 := 
RM       := rm -f

INCDIR	 = E:/devtools/usr/local/include
LIBDIR	 = E:/devtools/usr/local/lib

SRCS := test.cpp canvas.cpp
OBJS := $(SRCS:.cpp=.o)

.PHONY: clean all

$(info COMMON MAKEFILE)

all: compile link

compile: test.o canvas.o

link: 
	$(CC) $(OBJS) -L$(LIBDIR) -o main -lstdc++ -llibjpeg -ltga
	
test.o: test.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) -c test.cpp

canvas.o: canvas.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) -c canvas.cpp

clean:
	rm -f *.o main *.tga *.jpeg