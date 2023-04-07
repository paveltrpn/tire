
PKG_GLFW_INCDIR = ../../extern/glfw/include
PKG_GLFW_LIBDIR = ../../extern/glfw/build/src

PKG_FMT_INCDIR = ../../extern/fmt/include
PKG_FMT_LIBDIR = ../../extern/fmt/build

CC       = clang++-15
CFLAGS   = -Wall --std=c++20
LFLAGS 	 = -lfmt -lglfw3 -lpthread -ldl -lvulkan \
		   -lX11 -lXi -lXrandr -lXxf86vm \
		   -lXinerama -lXcursor -lrt -lm

INCDIR	 = 	$(PKG_GLFW_INCDIR) $(PKG_FMT_INCDIR)

LIBDIR	 = $(PKG_GLFW_LIBDIR) $(PKG_FMT_LIBDIR)

VPATH =  

SRCS = main.cpp instance.cpp devices.cpp

OBJS = $(patsubst %.cpp, %.o,$(SRCS))

#OBJS := $(SRCS:.cpp=.o)

.PHONY: clean all

all: $(OBJS) main

main.o: main.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

instance.o: instance.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

devices.o: devices.cpp
	$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@

main: $(OBJS)
	$(CC) $(OBJS) $(addprefix -L,$(LIBDIR)) -o main $(LFLAGS)

clean:
	rm -f *.o main
