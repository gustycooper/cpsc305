CC = gcc
CFLAGS = -g -Wall

default: program

program: main.o student.o split.o
	$(CC) $(CFLAGS) -o program main.o student.o split.o

main.o: main.c student.h split.h
	$(CC) $(CFLAGS) -c main.c

student.o: student.c student.h
	$(CC) $(CFLAGS) -c student.c

split.o: split.c split.h
	$(CC) $(CFLAGS) -c split.c

clean:
	rm -f program *.o
