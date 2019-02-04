CC          =	gcc

CFLAGS      =	 -Wall -g -std=c11 -g

all: list parser helper test

list: src/LinkedListAPI.c
	$(CC) $(CFLAGS) -c -fpic src/LinkedListAPI.c -Iinclude -o bin/LinkedListAPI.o
	gcc -shared -o bin/liblist.so bin/LinkedListAPI.o

parser: src/CalendarParser.c
	$(CC) $(CFLAGS) -c -fpic src/CalendarParser.c -Iinclude -o bin/CalendarParser.o
	gcc -shared -o bin/libcal.so bin/CalendarParser.o

helper: src/CalendarParserHelper.c
	$(CC) $(CFLAGS) -c -fpic src/CalendarParserHelper.c -Iinclude -o bin/CalendarParserHelper.o
	gcc -shared -o bin/libhelp.so bin/CalendarParserHelper.o

test: bin/liblist.so bin/libcal.so bin/libhelp.so
	$(CC) $(CFLAGS) bin/liblist.so bin/libcal.so bin/libhelp.so src/main.c -Iinclude -o bin/runMe

clean:
	rm bin/*.o bin/*.so
