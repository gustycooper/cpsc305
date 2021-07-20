CC = gcc
CFLAGS = -g -Wall
default: program
program: main.o point.o
	$(CC) $(CFLAGS) -o program main.o point.o
main.o: main.c point.h
	$(CC) $(CFLAGS) -c main.c
point.o: point.c point.h
	$(CC) $(CFLAGS) -c point.c
clean:
	rm -f program *.o
