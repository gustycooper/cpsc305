#include <stdio.h>
#include <string.h>
#include <locale.h> //unicode
#include <wchar.h> //unicode
#include "deck.h"
#include "card.h"
#include "player.h"

#define PLAYER_COUNT 2

// For now player.c defines players, and main accesses it
// Future - update playersInit to have parameters
extern Player players[PLAYER_COUNT]; //initialize players

const static char *RESET_COLOR = "\x1b[36m";

Card deck[DECK_SIZE];
Card *shuffled[DECK_SIZE];

int main(int argc, char **argv) {
    printf("%s",RESET_COLOR); //default color
    initCard(); // set up unicode printing
    initDeck(deck);
    shuffleDeck(shuffled, deck);
    Card *c = cutDeck(shuffled);
    printf("suit: %d, type: %d\n", c->suit, c->type);
    system("clear"); //clear console
    printf("Cut Card.\n");
    //c->hidden = 1;   // hidden==1 shows ???
    c->selected = 1; // changes card border color, 1 is red, 0 is green
    printCard(c, "\n");
    c->selected = 0; // make green for rest of deck
    printf("Deck before dealing:\n");
    printRestOfDeck(shuffled); // Printing the deck makes the cards visible
    playersInit();
    deal(shuffled, &players[0].hand,&players[1].hand,5);
    printf("%s Hand:\n", players[0].name);
    printCards(players[0].hand.cards, 0, 5);
    printf("%s Hand:\n", players[1].name);
    printCards(players[1].hand.cards, 0, 5);
    printf("Rest of deck:\n");
    printRestOfDeck(shuffled);
    printf("Draw top card from deck\n");
    printCard(draw(shuffled), "\n");
    printf("Rest of deck after draw:\n");
    printRestOfDeck(shuffled);

    printf("Some more Unicode characters\n\n");
    setlocale(LC_ALL, ""); //set up unicode printing
    int lets[] = { 0x461, 0x488, 0x489, 0x4ea, 0x4ec, 0x58d, 0x2ee3, 0x2ee4, 0x0 };
    for (int i = 0; i < sizeof(lets)/sizeof(int); i++)
        printf("%lc ", lets[i]);
    printf("\n");
    printf("%ls\n\n", lets);

}
