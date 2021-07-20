# makes chasm and charm_tools
# % make
# % make chasm
# % make charm_tools
all: chasm

FLAGS = -Wall
chasm: chasm.c chasm_types.h fsms.c fsms.h dict.c dict.h
	@echo 'building chasm.'
	gcc -o chasm chasm.c fsms.c dict.c $(FLAGS)

clean:
	rm -f chasm *.o

