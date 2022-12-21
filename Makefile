.PHONY:clean

CC = gcc
CFLAGS = -g -Wall

all: main.o ops.o common.o
	$(CC) $(CFLAGS) $^ -o vm 

%.o:%.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f *.o vm
