CC = gcc
CFLAGS = -std=c99 -Wall
OBJECTS = utils.o matrix.o

all: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) package.c -o package

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c -o utils.o

matrix.o: matrix.c matrix.h utils.o
	$(CC) $(CFLAGS) -c matrix.c -o matrix.o

test: all
	./package matrix1.dat matrix2.dat output.dat 3

check: all
	./package garbage.dat matrix2.dat output.dat 3
