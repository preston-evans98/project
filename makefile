CC = gcc
CFLAGS = -std=c99 -Wall
OBJECTS = utils.o matrix.o vector.o

all: package compute

compute: $(OBJECTS) compute.c
	$(CC) $(CFLAGS) $(OBJECTS) compute.c -o compute -lpthread -D_GNU_SOURCE

package: $(OBJECTS) package.c
	$(CC) $(CFLAGS) $(OBJECTS) package.c -o package -lpthread -D_GNU_SOURCE

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c -o utils.o

matrix.o: matrix.c matrix.h utils.o
	$(CC) $(CFLAGS) -c matrix.c -o matrix.o

vector.o: vector.c vector.h utils.o
	$(CC) $(CFLAGS) -c vector.c -o vector.o

runTests: $(OBJECTS) test.c package.c compute
	$(CC) $(CFLAGS) $(OBJECTS) package.c -o package -lpthread -DAUTO_TEST=1 -D_GNU_SOURCE
	$(CC) $(TESTFLAGS) $(OBJECTS) test.c -o test -D_GNU_SOURCE
	./test
	
test: all
	./package matrix1.dat matrix2.dat output.dat 2
	
check: all
	./package garbage.dat matrix2.dat output.dat 3

clean:
	rm *.o *.out package compute output.dat

stop: 
	pkill package
