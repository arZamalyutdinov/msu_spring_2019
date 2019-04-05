CC = g++ 
CFLAGS = -std=c++17 -O3 -c

all: test clean

test: test.o 
	$(CC) test.o -o test
	
test.o: test.cpp
	$(CC) $(CFLAGS) test.cpp

clean:
	rm -rf test.o