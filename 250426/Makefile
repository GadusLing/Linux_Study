CC = gcc
CFLAGS = -Wall -Wextra

all: test

test: test.o mystdio.o
	$(CC) $(CFLAGS) -o $@ $^

test.o: test.c mystdio.h
	$(CC) $(CFLAGS) -c test.c

mystdio.o: mystdio.c mystdio.h
	$(CC) $(CFLAGS) -c mystdio.c

clean:
	rm -f *.o test
