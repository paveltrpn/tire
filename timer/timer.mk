
CC     :=  g++
CFLAGS := -Wall -std=c++11 
LIBS   := -lstdc++ -lc 
RM     := rm -f


.PHONY: all clean

NAME := main
SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)

$(info TIMER MAKEFILE)

all: $(OBJS) $(NAME)

#
# link the .o files into the target executable
#
$(NAME): $(OBJS)
	$(CC) $^ -o $@ $(LIBS)

#
# compile the .cpp file into .o files using the compiler flags
#
%.o: %.cpp 
	$(CC) $(CFLAGS) -c $< -o $@ -I.


clean:
	$(RM) *.o
	$(RM) main
