# makes chasm and charm_tools
# % make
# % make chasm
# % make charm_tools
all: chasm charm_tools

FLAGS = -Wall
chasm: chasm.c chasm_types.h fsms.c fsms.h dict.c dict.h charmopcodes.h
	@echo 'building chasm.'
	gcc -o chasm chasm.c fsms.c dict.c $(FLAGS)

charm_tools: charm_tools.c
	@echo 'Building charm_tools.'
	gcc -o charm_tools charm_tools.c -Wall

clean:
	rm -f chasm charm_tools

