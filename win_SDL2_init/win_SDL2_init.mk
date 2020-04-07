SRCS := win_SDL2_init.cpp
OBJS := $(SRCS:.cpp=.o)

.PHONY: compile

compile: win_SDL2_init.o

win_SDL2_init.o: win_SDL2_init.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) -c win_SDL2_init.cpp
