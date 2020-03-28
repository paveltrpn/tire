
#Linux specific
#DEVHOME	= 
#SRCS		= $(wildcard *.cpp)
#OBJS		= $(SRCS:.cpp=.o)

#Windows specific
DEVHOME		= D:/Engineering

CC			= g++
CC_VERSION 	= $(shell g++ --version | grep g++)
CFLAGS		= -Wall -std=c++17
LIB_DIR 	+= D:\Devtools\MinGW-17.1\lib 
ALGEBRA2_CPP = $(DEVHOME)/algebra2_cpp

WIN_OPENGL_INIT_OBJS = win_opengl_init/win_opengl_init.o\
						$(ALGEBRA2_CPP)\mtrx2.o\
						$(ALGEBRA2_CPP)\mtrx3.o\
						$(ALGEBRA2_CPP)\mtrx4.o\
						$(ALGEBRA2_CPP)\vec2.o\
						$(ALGEBRA2_CPP)\vec3.o\
						$(ALGEBRA2_CPP)\vec4.o\
						$(ALGEBRA2_CPP)\qtnn.o\
						$(ALGEBRA2_CPP)\plane.o\



export CC
export CFLAGS

PRJ_LIST = img timer nix_opengl_init win_opengl_init

$(info $(CC_VERSION))

.PHONY: $(PRJ_LIST) clean all

all: $(PRJ_LIST)

win_opengl_init:
	cd $(ALGEBRA2_CPP) ; make -f algebra2.mk compile
	cd win_opengl_init ; make -f win_opengl_init.mk compile
	$(CC) -L$(LIB_DIR) $(WIN_OPENGL_INIT_OBJS) -o $@ -lstdc++ -lglfw3 -lopengl32 -lglu32 -lgdi32

img:
	cd img ; make compile
	$(CC) ip_test/ip_test.o -o $@ -lstdc++ 

clean:
	rm -f *.exe
	$(foreach prj, $(PRJ_LIST), rm -f $(prj)/*.o;)
	rm -f $(ALGEBRA2_CPP)/*.o

