CC := gcc
CFLAGS := -g -Wall -pedantic -O3
LDFLAGS :=

PROG := treap
OBJS := treap.o

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ treap.c

clean:
	rm -rf $(PROG) $(OBJS)

.PHONY: all clean

