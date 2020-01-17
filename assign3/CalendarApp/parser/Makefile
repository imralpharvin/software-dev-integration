CC          =	gcc

CFLAGS      =	 -Wall -g -std=c11

all: parser

test: parser main

parser: src/CalendarParser.c  src/LinkedListAPI.c
		$(CC) $(CFLAGS) -c -fpic src/CalendarParser.c -Iinclude -o ../CalendarParser.o
		$(CC) $(CFLAGS) -c -fpic src/LinkedListAPI.c -Iinclude -o ../LinkedListAPI.o
		gcc -shared -o ../libcal.so ../CalendarParser.o ../LinkedListAPI.o

main: ../libcal.so
	$(CC) $(CFLAGS) ../libcal.so src/main.c -Iinclude -o test

clean:
	rm bin/*.o bin/*.so bin/*
