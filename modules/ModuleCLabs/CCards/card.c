#include <stdio.h>
#include <string.h>
#include <locale.h> //unicode
#include <wchar.h> //unicode
#include "card.h"

//escape sequences for changing colors
const char *BLACK = "\x1B[30m";
const char *RED = "\x1B[31m";
const char *GREEN = "\x1B[32m";
const static char *RESET_COLOR = "\x1b[36m";

//don't change these
const static int CARD_WIDTH = 3; //minimum 3 (adds two for edges)
//const static int CARD_HEIGHT = 3;

static int suitSymbol[] = {0x2660,0x2665,0x2666,0x2663}; //unicode values
static int wallSymbol[] = {0x2550,0x2551,0x2554,0x2557,0x255A,0x255D}; //unicode symbols for boarders


//print special character (unicode)
static void printWchar(wchar_t c) {
    printf("%lc",c);
    //putwchar(c);
}

int printPartOfCard(Card *card, int partOfCard) {
    //compiler throws warning if not const
    const char *color = (card->selected) ? RED : GREEN;
    printf("%s",color); //change color of text
    switch (partOfCard) {
        case 0: //top row
            printWchar(wallSymbol[TOP_LEFT]);
            for (int i = 0; i < CARD_WIDTH; i++) {
                printWchar(wallSymbol[HORIZONTAL]);
            }
            printWchar(wallSymbol[TOP_RIGHT]);
            break;
        case 1: //center row
            printWchar(wallSymbol[VERTICAL]);
            if (card->hidden) { //print question marks
                for (int i = 0; i < CARD_WIDTH; i++) {
                    printf("%c",'?');
                }
            } else { //print suit|value|suit
                if (card->suit == SPADE || card->suit == CLUB)
                    printf("%s", BLACK);
                else
                    printf("%s", RED);
                printWchar(suitSymbol[card->suit]);
                if (card-> type == 1) {
                    printf("%c",'A'); //Ace
                } else if (card->type < 10){
                    printf("%d",card->type);
                } else if (card-> type == 10) {
                    printf("%c",'X'); //10 is two characters and would mess things up
                } else if (card->type == 11) {
                    printf("%c",'J');
                } else if (card->type == 12) {
                    printf("%c",'Q');
                } else if (card->type == 13) {
                    printf("%c",'K');
                }
                printWchar(suitSymbol[card->suit]);
            }
            printf("%s",color); //change color of text
            printWchar(wallSymbol[VERTICAL]);
            break;
        case 2:
            printWchar(wallSymbol[BOTTOM_LEFT]);
            for (int i = 0; i < CARD_WIDTH; i++) {
                printWchar(wallSymbol[HORIZONTAL]);
            }
            printWchar(wallSymbol[BOTTOM_RIGHT]);
            //printf("%s",RESET_COLOR); //reset the color after the last part has been printed
            break;
    }
    printf("%s",RESET_COLOR); //default color
    return CARD_WIDTH +2; //amount of characters printed
}

int printCard(Card *card, char *aftercard) {
    for (int x = 0; x < 3; x++) {
        printPartOfCard(card, x);
        printf("%s", aftercard);
    }
    return 0; // fix this
}

/*
For now printCards turns off c->hidden so all cards are displayed.
Future games will want printCards to honor the current value of c->hidden
Future games will provide alternative means to turn off c->hidden
See player.c revealCards() for one way to turn off c->hidden
*/
void printCards(Card **cards, int i, int j) {
    Card *c;
    for (int y = i; y < j; y++) {
        c = cards[y];
        printPartOfCard(c, 0);
    }
    printf("\n");
    for (int y = i; y < j; y++) {
        c = cards[y];
        //system("clear"); //clear console
        c->hidden = 0;   // hidden==1 shows ???
        c->selected = 0; // changes card border color
        printPartOfCard(c, 1);
    }
    printf("\n");
    for (int y = i; y < j; y++) {
        c = cards[y];
        printPartOfCard(c, 2);
    }
    printf("\n");
}

void initCard() {
    setlocale(LC_ALL, ""); //set up unicode printing
}
