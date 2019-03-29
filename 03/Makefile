CC = g++ 
CFLAGS = -O2 -c

all: test clean

test: test.o matrix.o
	$(CC) test.o matrix.o -o test

test.o: test.cpp
	$(CC) $(CFLAGS) test.cpp

matrix.o: matrix.cpp
	$(CC) $(CFLAGS) matrix.cpp

clean:
	rm -rf *.o