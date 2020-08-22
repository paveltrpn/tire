
CC       := tsc
CFLAGS   := 

SRCS := src/test.ts
		
OBJS := test.js
		
#OBJS := $(SRCS:.ts=.js)

.PHONY: clean all

all: $(OBJS)

test.js: src/test.ts
	$(CC) $(CFLAGS) --outFile test.js src/test.ts

clean:
	rm -f *.js
