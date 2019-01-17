CC          =	gcc

CFLAGS      =	-Wall -std=c99 -pedantic -g


main: src/main.c src/CalendarParser.c src/LinkedListAPI.c
	$(CC) $(CFLAGS) src/main.c src/CalendarParser.c src/LinkedListAPI.c -Iinclude -o bin/runMe

clean:
	rm bin/*
