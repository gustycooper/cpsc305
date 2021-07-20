#include <string.h>
#include "player.h"

const char *const COMPUTER_NAME = "Computer";
char *const PLAYER_NAME = "Player";
Player players[PLAYER_COUNT]; //initialize players

//makes all cards visible
void revealCards(Hand* hand ) {
    for (char i = 0; i < hand->cardsInHand; i++) {
        hand->cards[i]->hidden = false;
    }
}

//makes all cards in hand hidden
void hideCards(Hand* hand ) {
    for (char i = 0; i < hand->cardsInHand; i++) {
        hand->cards[i]->hidden = true;
    }
}

void playersInit() {
    players[0].score=0;
    players[1].score=0;
    strcpy(players[0].name,COMPUTER_NAME);
    strcpy(players[1].name,PLAYER_NAME);
}
