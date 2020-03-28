INC_DIR += -ID:\Devtools\MinGW-17.1\include
INC_DIR += -ID:\Engineering\algebra2_cpp\

SRCS := win_opengl_init.cpp
OBJS := $(SRCS:.cpp=.o)

compile: win_opengl_init.o

win_opengl_init.o: win_opengl_init.cpp
	$(CC) $(CFLAGS) $(INC_DIR) -c win_opengl_init.cpp