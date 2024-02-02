CC=gcc
CFLAGS=-g -Wall -Werror

all: bin/shell bin/ls

bin/shell: shell.o 
	gcc -o $@ $^

bin/ls: ls.o
	gcc -o $@ $^

clean:
	rm -f *.o bin/*
