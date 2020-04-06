SRCS := image.cpp mainApp.cpp test.cpp
OBJS := $(SRCS:.cpp=.o)


compile: image.o mainApp.o test.o

image.o: image.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) -c image.cpp

mainApp.o: mainApp.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) -c mainApp.cpp 

test.o: test.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) -c test.cpp