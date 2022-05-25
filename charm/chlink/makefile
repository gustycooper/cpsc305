# makes chlinker
# % make
# % make chlinker
all: chlinker

FLAGS = -Wall
chlinker: chlink.c chlink.h dict.c dict.h
	@echo 'building chasm.'
	gcc -o chlinker chlink.c dict.c $(FLAGS)

clean:
	rm -f chlinker

