CC=clang
CFLAGS=-g -Wall

all: bincalc dllist.o

bincalc: bincalc.c dllist.o
	$(CC) $(CFLAGS) dllist.o bincalc.c -o bincalc

dllist.o: dllist.c dllist.h
	$(CC) $(CFLAGS) -c dllist.c dllist.h
clean: 
	rm bincalc dllist.o
