
#Linux specific
#DEVHOME	= 
#SRCS		= $(wildcard *.cpp)
#OBJS		= $(SRCS:.cpp=.o)

#Windows specific
DEVHOME		= D:/Engineering
ALGEBRA_CPP = $(DEVHOME)/algebra_cpp


CC			= g++
CC_VERSION 	= $(shell g++ --version | grep g++)
CFLAGS		= -Wall -std=c++17
LIB_DIR 	+= D:\Devtools\MinGW-17.1\lib 

export CC
export CFLAGS
export ALGEBRA_CPP

PRJ_LIST = img timer nix_opengl_init win_opengl_init

$(info $(CC_VERSION))

.PHONY: $(PRJ_LIST) clean all

all: $(PRJ_LIST)

win_opengl_init:
	cd win_opengl_init ; make -f win_opengl_init.mk compile
	$(CC) -L$(LIB_DIR) win_opengl_init/win_opengl_init.o -o $@ -lstdc++ -lglfw3 -lopengl32 -lglu32 -lgdi32

img:
	cd img ; make compile
	$(CC) ip_test/ip_test.o -o $@ -lstdc++ 

clean:
	rm -f *.exe
	$(foreach prj, $(PRJ_LIST), rm -f $(prj)/*.o;) 

