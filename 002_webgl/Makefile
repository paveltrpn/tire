
CC       := tsc
CFLAGS   :=

SRCS := src/test.ts \
		src/mtrx4.ts
		
OBJS := test.js mtrx4.js
		
#OBJS := $(SRCS:.ts=.js)

.PHONY: clean all

all: $(OBJS)

mtrx4.js: src/mtrx4.ts
	$(CC) $(CFLAGS) --outFile mtrx4.js src/mtrx4.ts

test.js: src/test.ts
	$(CC) $(CFLAGS) --outFile test.js src/test.ts

clean:
	rm -f *.js
