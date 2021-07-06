CC       = g++-11
INCDIR	 := 
LIBDIR	 := ../3d_party/bullet3/lib
CFLAGS   = -Wall -Wextra -Wpedantic -Wno-unused-parameter -std=c++20
LFLAGS 	 = -lstdc++ -lfmt -lBulletSoftBody -lBulletDynamics -lBulletCollision -lLinearMath

SOURCE_PREFIX = src ../algebra ../3d_party/bullet3/include/bullet/
BUILD_PREFIX = build

VPATH := $(SOURCE_PREFIX)

SRCS := main.cpp

OBJS := $(patsubst %.cpp,$(BUILD_PREFIX)/%.o,$(SRCS))

#OBJS := $(SRCS:.cpp=.o)

.PHONY: clean all

all: $(OBJS) main

$(BUILD_PREFIX)/main.o: main.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

main: $(OBJS)
	$(CC) $(OBJS) -L$(LIBDIR) $(LFLAGS) -o main 

clean:
	rm -f build/*.o main
