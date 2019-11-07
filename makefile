CC = gcc
CFLAGS = -std=c99 -Wall
OBJECTS = utils.o matrix.o

all: package compute

compute: $(OBJECTS) compute.c
	$(CC) $(CFLAGS) $(OBJECTS) compute.c -o compute -lpthread -DDEBUG=1

package: $(OBJECTS) package.c
	$(CC) $(CFLAGS) $(OBJECTS) package.c -o package -lpthread -DDEBUG=1

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c -o utils.o

matrix.o: matrix.c matrix.h utils.o
	$(CC) $(CFLAGS) -c matrix.c -o matrix.o

test: clear all 
	./package matrix1.dat matrix2.dat output.dat 0
	# ./compute 3

check: all
	./package garbage.dat matrix2.dat output.dat 3

clean:
	rm *.o package compute output.dat

clear: all
	./clearQueue

stop: clear
	pkill package
