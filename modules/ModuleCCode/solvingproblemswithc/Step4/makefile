CC = gcc           # define gcc as compiler
CFLAGS = -g -Wall  # define compiler flags, -g is debug, -Wall turns on all warngings

default: part      # name of our program

# part is dependent on main.o and part.o
# if main.o or part.o have a date newer than part, invoke rule
part: main.o part.o
	$(CC) $(CFLAGS) -o part main.o part.o

# main.o is dependent on main.c and part.h
# if main.c or part.h have a date newer than main.o, invoke rule
main.o: main.c part.h
	$(CC) $(CFLAGS) -c main.c

# part.o is dependent on part.c and part.h
# if part.c or part.h have a date newer than part.o, invoke rule
part.o: part.c part.h
	$(CC) $(CFLAGS) -c part.c

# enter $ make clean to delete part and all .o files
clean:
	rm -f part *.o
