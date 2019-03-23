CC = g++ 
CFLAGS = -O2 -c

all: test clean

test: main.o calc.o
	$(CC) main.o calc.o -o test

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

calc.o: calc.cpp
	$(CC) $(CFLAGS) calc.cpp

clean:
	rm -rf *.o