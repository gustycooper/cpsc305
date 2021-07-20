#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "deck.h"
#include "card.h"
#include "player.h"

//Initial deck has cards hidden. Hidden cards are printed as ???
bool HIDE_CARDS = true;

//initialize deck
// Creates a Card[]
void initDeck(Card *deck) {
    char deckIndex = 0; //index of deck from 0-51
    //iterate through all suits
    for (Suit suit = SPADE; suit <= CLUB; suit++) {
        //iterate through all numbers
        for (char number = 1; number <= KING; number++) {
            //initialize all variables
            deck[deckIndex].suit = suit;
            deck[deckIndex].type = number;
            deck[deckIndex].hidden = HIDE_CARDS;
            deck[deckIndex].selected = false;
             //value either number value, or 10 for face card
            deck[deckIndex].value = (number > 10) ? 10: number;
            deckIndex++; //increase to next slot
        }
    }
}

/*
Shuffle pointers to each card
Creates a *Card[] from a Card[]
*/
void shuffleDeck(Card **shuffled, Card *deck) {
    srand(time(NULL));// seed random with time
    //clear shuffled slots first
    memset(shuffled,0,sizeof(Card*)*DECK_SIZE); //set each pointer to NULL
    //for all 52 cards
    for (char i = 0; i < DECK_SIZE; i++) {
        //generate a random slot from 0-1
        char r = rand()%DECK_SIZE;
        //if there is already a card pointer at this slot,move up one slot
        //if it goes to 52, becomes 0, etc
        for (;shuffled[r%DECK_SIZE];r++){}
        //make random slot point to the card
        shuffled[r%DECK_SIZE] = &(deck[i]); //pointer to a card
    }
}

/*
Cut a card from the deck
from startingAt, until end of deck
set that card as cut card
*/
Card *cutDeck(Card **shuffled) {
    //this just picks the next card that isn't dealt
    Card *topCard = shuffled[rand()%DECK_SIZE];
    return topCard;
}

char cardIndex = 0; // This could be part of Deck
/*
deal cards into each hand
alternate putting deck pointers into each hand
*/
void deal(Card **shuffled, Hand* hand0,Hand* hand1,int hand_size) {
    for (char i = 0; i < hand_size; i++) {
        //set the card ptr in each hand
        //increase the index in deck
        hand0->cards[i] = shuffled[cardIndex++];
        hand0->cardsInHand++; //increase number of cards
        hand1->cards[i] = shuffled[cardIndex++];
        hand1->cardsInHand++; // increase number of cards
    }
}

Card *draw(Card **shuffled) {
    return shuffled[cardIndex++];
};

void printRestOfDeck(Card **shuffled) {
    int endIndex = cardIndex+13 >= DECK_SIZE ? DECK_SIZE : cardIndex+13;
    printCards(shuffled, cardIndex, endIndex);
    if (endIndex >= DECK_SIZE) goto done;
    endIndex = cardIndex+26 >= DECK_SIZE ? DECK_SIZE : cardIndex+26;
    printCards(shuffled, cardIndex+13, endIndex);
    if (endIndex >= DECK_SIZE) goto done;
    endIndex = cardIndex+39 >= DECK_SIZE ? DECK_SIZE : cardIndex+39;
    printCards(shuffled, cardIndex+26, endIndex);
    if (endIndex >= DECK_SIZE) goto done;
    endIndex = DECK_SIZE;
    printCards(shuffled, cardIndex+39, endIndex);
done:
    return;
}
