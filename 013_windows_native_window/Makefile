################################
# Windows-специфичный Makefile #
################################
CC       := g++
CFLAGS   := -Wall -std=c++20

INCDIR	 = E:/devtools/usr/local/include
LIBDIR	 = E:/devtools/usr/local/lib

LFLAGS 	 = -lstdc++ -lgdi32 -lUser32 -lfmt

SOURCE_PREFIX = src

BUILD_PREFIX = build

VPATH := $(SOURCE_PREFIX)

SRCS = main.cpp
	  
OBJS := $(patsubst %.cpp,$(BUILD_PREFIX)/%.o,$(SRCS))

.PHONY: clean all

all: main

$(BUILD_PREFIX)/%.o: %.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

main: $(OBJS)
	$(CC) $^ -L$(LIBDIR) -o $@ $(LFLAGS)

clean:
	rm -f $(BUILD_PREFIX)/*.o main
