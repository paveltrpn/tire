
CC       := tsc
CFLAGS   := -t ES2016 --removeComments

SRCS := src/test.ts \
		src/mtrx4.ts \
		src/vec3.ts \
		src/common.ts
		
OBJS := test.js test2.js
		
#OBJS := $(SRCS:.ts=.js)

.PHONY: clean all

all: $(OBJS)

test.js: $(SRCS)
	$(CC) $(CFLAGS) --outFile test.js src/test.ts src/mtrx4.ts src/vec3.ts src/common.ts

test2.js: $(SRCS)
	$(CC) $(CFLAGS) --outFile test2.js src/test2.ts src/mtrx4.ts src/vec3.ts src/common.ts

clean:
	rm -f *.js
