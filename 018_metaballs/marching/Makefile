CC       = g++
CFLAGS   = -Wall 
LFLAGS 	 = -lstdc++ -lopengl32 -lfreeglut
INCDIR	 := ../../extern/glut/include/ 
LIBDIR	 := ../../extern/glut/lib/X64/

SRCS := marching.cpp
OBJS := $(patsubst %.cpp,%.o,$(SRCS))

.PHONY: clean all

all: $(OBJS) main

marching.o: marching.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

main: $(OBJS)
	$(CC) marching.o $(addprefix -L,$(LIBDIR))  -o main $(LFLAGS)

clean:
	rm -f *.o main