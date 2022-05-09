# makes two versions of cheme
# chemut - terminal version, command line
# chemun - ncurses version, pseudo windows
# % make
all: chemun chemut

FLAGS = -Wall -lncurses
CPPFLAGS = -D NCURSES
chemun: chemu.c cpu.c cpu.h memory.c memory.h command.c isa.c isa.h bit_functions.c bit_functions.h dict.c dict.h charmopcodes.h
	@echo 'building chemun, ncurses chemu.'
	gcc -o chemun chemu.c cpu.c memory.c command.c isa.c bit_functions.c dict.c $(FLAGS) $(CPPFLAGS)

chemut: chemu.c cpu.c cpu.h memory.c memory.h command.c isa.c isa.h bit_functions.c bit_functions.h dict.c dict.h charmopcodes.h
	@echo 'Building chemut, command line chemu.'
	gcc -o chemut chemu.c cpu.c memory.c command.c isa.c bit_functions.c dict.c -Wall

clean:
	rm -f chemun chemut

