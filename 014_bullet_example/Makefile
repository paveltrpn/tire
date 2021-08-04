CC       = g++
INCDIR	 := E:/devtools/usr/local/include
LIBDIR	 := -LE:/devtools/usr/local/lib -L../3d_party/bullet3/lib -LE:/devtools/usr/local/lib
CFLAGS   = -Wall -Wextra -Wpedantic -Wno-unused-parameter -std=c++20
LFLAGS 	 = -lstdc++ -lopengl32 -lglu32 -lglfw3 -lgdi32 -lglew32 -ltga -llibjpeg -lfmt -lBulletSoftBody -lBulletDynamics -lBulletCollision -lLinearMath

SOURCE_PREFIX = src ../3d_party/bullet3/include/bullet/ ../3d_party/
BUILD_PREFIX = build

VPATH := $(SOURCE_PREFIX)

SRCS := main.cpp \
		apputils.cpp \
		bitmap.cpp \
		mesh.cpp \
		matbase.cpp \
		camera.cpp \
		timing.cpp \
		screen_text.cpp

OBJS := $(patsubst %.cpp,$(BUILD_PREFIX)/%.o,$(SRCS))

#OBJS := $(SRCS:.cpp=.o)

.PHONY: clean all

all: $(OBJS) main

$(BUILD_PREFIX)/main.o: main.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

$(BUILD_PREFIX)/apputils.o: apputils.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

$(BUILD_PREFIX)/bitmap.o: bitmap.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

$(BUILD_PREFIX)/mesh.o: mesh.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

$(BUILD_PREFIX)/matbase.o: matbase.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

$(BUILD_PREFIX)/camera.o: camera.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

$(BUILD_PREFIX)/timing.o: timing.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

$(BUILD_PREFIX)/screen_text.o: screen_text.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) $(addprefix -I,$(SOURCE_PREFIX)) -c $< -o $@

main: $(OBJS)
	$(CC) $(OBJS) -L$(LIBDIR) -o main $(LFLAGS)

clean:
	rm -f build/*.o main