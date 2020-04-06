SRCS := bmp.cpp tga.cpp jpeg.cpp
OBJS := $(SRCS:.cpp=.o)


compile: bmp.o tga.o jpeg.o

bmp.o: bmp.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) -c bmp.cpp

tga.o: tga.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) -c tga.cpp

jpeg.o: jpeg.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) -c jpeg.cpp
