SRCS := image.cpp mainApp.cpp test.cpp
OBJS := $(SRCS:.cpp=.o)

compile: image.o mainApp.o test.o

image.o: image.cpp
	$(CC) $(CFLAGS) -c image.cpp

mainApp.o: mainApp.cpp
	$(CC) $(CFLAGS) -c mainApp.cpp 

test.o: test.cpp
	$(CC) $(CFLAGS) -c test.cpp