.PHONY:clean

CC = gcc
CFLAGS_TARGET = -g
CFLAGS = -g -c

all: common.o ops.o main.o
	$(CC) $(CFLAGS_TARGET) common.o ops.o main.o -o vm 

main.o: main.c
	$(CC) $(CFLAGS) main.c -o main.o

common.o: common.c common.h
	$(CC) $(CFLAGS) common.c common.h -o common.o

ops.o: ops.c ops.h
	$(CC) $(CFLAGS) ops.c ops.h -o ops.o 
	
clean:
	rm -f *.o