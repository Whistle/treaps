CC := gcc
CFLAGS := -Wall -pedantic -O3

default: treap.o bsp.o 
	$(CC) $(CFLAGS) -o treap treap.o bsp.o
treap.o: treap.c treap.h
	$(CC) $(CFLAGS) -c treap.c
bsp.o: bsp.c treap.h
	$(CC) $(CFLAGS) -c bsp.c
clean:
	rm -rf *.o 

.PHONY: clean

