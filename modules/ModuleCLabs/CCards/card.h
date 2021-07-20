#ifndef CARD_H
#define CARD_H

#include <stdbool.h> //booleans

#define JACK 11
#define QUEEN 12
#define KING 13

/*
card.c uses values from Suit and Walls as indices into arrays suitSymbol and WallSymbol 
*/
typedef enum suit {SPADE,HEART,DIAMOND,CLUB} Suit;
typedef enum walls {HORIZONTAL,VERTICAL,TOP_LEFT,TOP_RIGHT,BOTTOM_LEFT,BOTTOM_RIGHT} Walls;

/*
Card structure. Each card has a suit and type
Additionally, it can either be hidden or revealed
One card at a time may also be selected
*/
typedef struct{
    Suit suit; //suit of card
    unsigned char type; //Ace-9, J,Q,K
    bool hidden; //whether the cards value is shown or hidden
    bool selected; //whether current card is selected
    unsigned char value; //1-10
} Card;

void initCard();
int printCard(Card *card, char *aftercard);
void printCards(Card **card, int i, int j);
int printPartOfCard(Card *card, int partOfCard);

#endif
