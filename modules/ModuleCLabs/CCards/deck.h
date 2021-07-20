#ifndef DECK_H
#define DECK_H
#include "card.h"
#include "player.h"

#define DECK_SIZE 52 // #cards
#define CUT_BUFFER 4 //number of cards on either side that need to be left

void initDeck(Card *deck); //create initial deck
void shuffleDeck(Card **shuffled, Card *deck); //arrange card pointers in shuffled randomly
void deal(Card **shuffled, Hand*,Hand*,int); //deal 6 cards from deck to each hand
Card *cutDeck(Card **shuffled); //select a random card from startingAt, until end of deck
Card *draw(Card **shuffled); // draw the top card
void printRestOfDeck(Card **shuffled);

#endif
