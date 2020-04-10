
#Linux specific
#DEVHOME	= 
#SRCS		= $(wildcard *.cpp)
#OBJS		= $(SRCS:.cpp=.o)

#Windows specific
DEVHOME			:= D:/Engineering
DEVTOOLS		:= D:/Devtools
ALGEBRA_CPP 	:= $(DEVHOME)/algebra_cpp
BOOSTDIR		:= $(DEVTOOLS)/boost_1_72_0
BOOSTLIBDIR		:= $(DEVHOME)/boost_1_72_0/lib64-msvc-14.2
INCDIR			:= $(DEVTOOLS)\\MinGW-17.1\\include
LIBDIR 			+= $(DEVTOOLS)\\MinGW-17.1\\lib 

ALGEBRA2_CPP 	:= $(DEVHOME)/algebra2_cpp
SDL2LIBS		:= -lSDL2 -ljpeg -lgdi32 -limm32 -lole32 -loleaut32 -lVersion -lSetupapi -lsecur32 -lwinmm -ldmoguids -lwmcodecdspuuid -lmsdmo -lStrmiids

CC_VERSION = $(shell g++ --version | grep g++)
CC		= g++
CFLAGS	= -g -Wall -std=c++17

export CC
export CFLAGS
export INCDIR

WIN_OPENGL_INIT_OBJS = win_opengl_init/win_opengl_init.o\
						$(ALGEBRA2_CPP)\mtrx2.o\
						$(ALGEBRA2_CPP)\mtrx3.o\
						$(ALGEBRA2_CPP)\mtrx4.o\
						$(ALGEBRA2_CPP)\vec2.o\
						$(ALGEBRA2_CPP)\vec3.o\
						$(ALGEBRA2_CPP)\vec4.o\
						$(ALGEBRA2_CPP)\qtnn.o\
						$(ALGEBRA2_CPP)\plane.o

WIN_SDL2_INIT_OBJS = img/tga.o\
					 img/bmp.o\
					 img/jpeg.o\
					 win_SDL2_init/win_SDL2_init.o

PRJ_LIST = img timer nix_opengl_init win_opengl_init win_SDL2_init

$(info $(CC_VERSION))

.PHONY: $(PRJ_LIST) clean all

all: $(PRJ_LIST)

win_opengl_init:
	cd $(ALGEBRA2_CPP) ; make -f algebra2.mk compile
	cd win_opengl_init ; make -f win_opengl_init.mk compile
	$(CC) -L$(LIBDIR) $(WIN_OPENGL_INIT_OBJS) -o $@ -lstdc++ -lglfw3 -lopengl32 -lglu32 -lgdi32

win_SDL2_init:
	cd img ; make -f img.mk compile
	cd win_SDL2_init; make -f win_SDL2_init.mk compile
	$(CC) -L$(LIBDIR) $(WIN_SDL2_INIT_OBJS) -o $@ -lstdc++ $(SDL2LIBS)

clean:
	rm -f *.exe
	$(foreach prj, $(PRJ_LIST), rm -f $(prj)/*.o;)
	rm -f $(ALGEBRA2_CPP)/*.o

