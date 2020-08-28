
CC       := tsc
CFLAGS   := -t ES2016 --removeComments

SRCS := src/test2.ts \
		src/mtrx4.ts \
		src/vec3.ts \
		src/common.ts
		
OBJS := 
		
#OBJS := $(SRCS:.ts=.js)

.PHONY: clean all

all: cube.js

cube.js: $(SRCS)
	$(CC) $(CFLAGS) --outFile cube.js $(SRCS)

clean:
	rm -f *.js
