# makes card_demo
# % make
# % make card_demo
# % make clean
all: card_demo

FLAGS = -Wall -Wno-char-subscripts
card_demo: main.c card.h card.c deck.c deck.h player.c player.h
	@echo 'building card_demo.'
	gcc -o card_demo main.c card.c deck.c player.c $(FLAGS)

clean:
	rm -f card_demo *.o

