INC_DIR += D:\Devtools\MinGW-17.1\include\

SRCS := win_opengl_init.cpp
OBJS := $(SRCS:.cpp=.o)

compile: win_opengl_init.o

win_opengl_init.o: win_opengl_init.cpp
	$(CC) $(CFLAGS) -I$(INC_DIR) -c win_opengl_init.cpp