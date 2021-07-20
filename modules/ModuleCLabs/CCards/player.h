#ifndef PLAYER_H
#define PLAYER_H

#include "card.h"
#include <stdlib.h>
#include <unistd.h>

#define NAME_LENGTH 10 //# char permissible in a player name
#define HAND_SIZE 10 // max cards in hand
//#define ENTER 
#define PLAYER_COUNT 2

typedef struct {
    Card *cards[HAND_SIZE]; //the cards the hand holds
    char cardsInHand; //number of cards in a hand
} Hand;

typedef struct{
    char name[NAME_LENGTH];
    Hand hand; //array of card "slots" (ptrs), & points
    char selectedCard; //selected card index
    unsigned char score; //score
} Player;

void playersInit();

#endif
