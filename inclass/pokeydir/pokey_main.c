#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <ncurses.h>
#include "pokmon.h"

// TODO
// 1. create a pokey_win.h and pokey_win.c
// 2. create functions for found pokcell and found fightcell
// 3. create functions to the various commands f, s, etc.

/********************************************************************************
 * Window Section of pokey_main
 * This section has defines, variables, and functions to manipulate the windows
 ********************************************************************************/ 

// Defines for minimal terminal size
#define MINLINES 41
#define MINCOLS 75

// Defines for Results window and Pokey Play window
// Windows have 
// WID - number of chars (horizontal - x coordinates)
// HGT - number of lines (vertical - y coordinates)
// LIN - start line in window
// COL - start column in window

// Results Window defines
#define RESWID 70
#define RESHGT 15
#define RESLIN 1
#define RESCOL 1
#define RESDISLINES 12
#define RESVALSLINES 50

// Pokey Play Window defines
#define POKWID 70
#define POKHGT 23
#define POKLIN 16
#define POKCOL 1

// Define our window variables
static WINDOW *mainwin, *pokwin, *reswin;

// px and py are indices into two program components
// 1. the position of the player's letter in Pokey Play window
// 2. The position of the player's letter in the struct pokmon *pokmon[][]
//    pokmon[][] contains game info. See defintion below
//
// px and py are define as static so two functions can use them.
// px and py are defined here so updatepokwin() can use them.
// One could pass px, py to updatepokwin() and make them more local
static int px = 5, py = 5;

// Pokey Play window is 22 Lines, enumerated as 0 through 21
// Each line is 70 characters long.
// Line 0 displays Pokey Play
// Line 21 is for commands. It has a > followed by the cursor
// Lines 1 through 20 are for moving the player with arrow keys
// As you move you are searching a Pokemons
// Currently Pokey Play window line numbers are magic numbers
static char pokwinln[POKWID] = "                                                                    ";
static char pokwincmd[POKWID] = ">                                                                   ";
void updatepokwin(WINDOW *pokwin, char *command) {
    strcpy(pokwinln,"                            Pokey Play                              ");
    char px10 = '0' + px / 10;
    char px_1 = '0' + px % 10;
    char py10 = '0' + py / 10;
    char py_1 = '0' + py % 10;
    pokwinln[50] = px10;
    pokwinln[51] = px_1;
    pokwinln[54] = py10;
    pokwinln[55] = py_1;
    mvwprintw(pokwin, 1, 1, "%s", pokwinln);
    for (int i = 0; i < 19; i++) {
        strcpy(pokwinln, "                                                                    ");
        if (i == py)
            pokwinln[px] = 'P';
        else
            pokwinln[px] = ' ';
        mvwprintw(pokwin, i+2, 1, "%s", pokwinln);
    }
    strcpy(&pokwincmd[2], "                                                                  ");
    strcpy(&pokwincmd[2], command);
    mvwprintw(pokwin, 21, 1, "%s", pokwincmd);
}
void create_pokey_play_win() {
    pokwin = subwin(mainwin, POKHGT, POKWID, POKLIN, POKCOL);
    box(pokwin, 0, 0);
    wattron(pokwin, A_BOLD);
    mvwaddstr(pokwin, 1, 1, "                            Pokey Play                    ");
    updatepokwin(pokwin, "start");
}


// reswin is used to display information
// printres() is called to put information in reswin, which takes 3 steps to display
// 1. printres() actually puts information is the resvals[][] array
// 2. updatereswin() updates the contents of reswin
// 3. wrefresh(reswin) refreshes the display in reswin
//
// resvals is circular buffer of 50 lines, with 12 of them displayed at one time
// The reswin has 12 lines of display
// A future enhance can be a scrollable reswin using the circular buffer
char resvals[RESVALSLINES][80] = {
    {"                                      "}, {"                                      "},
    {"                                      "}, {"                                      "},
    {"                                      "}, {"                                      "},
    {"                                      "}, {"                                      "},
    {"                                      "}, {"                                      "},
    {"                                      "}, {"                                      "},
    {"                                      "}, {"                                      "},
    {"                                      "}, {"                                      "},
    {"                                      "}, {"                                      "},
    {"                                      "}, {"                                      "},
    {"                                      "}, {"                                      "},
    {"                                      "}, {"                                      "},
    {"                                      "}, {"                                      "},
    {"                                      "}, {"                                      "},
    {"                                      "}, {"                                      "},
    {"                                      "}, {"                                      "},
    {"                                      "}, {"                                      "},
    {"                                      "}, {"                                      "},
    {"                                      "}, {"                                      "},
    {"                                      "}, {"                                      "},
    {"                                      "}, {"                                      "},
    {"                                      "}, {"                                      "},
    {"                                      "}, {"                                      "},
    {"                                      "}, {"                                      "},
    {"                                      "}, {"                                      "},
    };

// resi and resn are used by printres and updatereswin to maintain reswin
static int resi = 0; // count of lines added since last update
static int resn = 0; // where to place line in resvals[], circular buffer

// printres has variable arguments just like fprintf
__attribute__((format(printf, 1, 2)))
void printres(char *fmt, ...) {
  char res[80];
  va_list ap;
  va_start(ap, fmt);
  vsprintf(res, fmt, ap);
  va_end(ap);
  strcpy(resvals[resn], "                                                                ");
  strcpy(resvals[resn], res);
  resn = (resn + 1) % RESVALSLINES; // place in the resvals buffer
  resi++;
}

int scrn = 0; // where to begin scrolling
void updatereswin(WINDOW *reswin) {
    int disi = resn -  RESDISLINES; // resvals[disi] is start of display
    if (disi < 0) // wrap around resvals[]
        disi = RESVALSLINES + disi;
    for (int i = 0; i < RESDISLINES; i++) {
        mvwprintw(reswin, i+2, 1, "%s", "                                                                ");
        mvwprintw(reswin, i+2, 1, "%s", resvals[disi]);
        disi = (disi + 1) % RESVALSLINES;
    }
    resi = 0;
    scrn = resn; // establish scroll position
}

void create_results_win() {
    reswin = subwin(mainwin, RESHGT, RESWID, RESLIN, RESCOL);
    box(reswin, 0, 0);
    wattron(reswin, A_BOLD);
    mvwaddstr(reswin, 1, 1, "                             Results                      ");
    updatereswin(reswin);
}

/****************************************************************************
 * Game Play section of pokey_main
 * This section defines variables and functions to play the game.
 ****************************************************************************/ 

// Define data structure that mirrors the Pokey Play window
// Pokey Play window is 70 chars wide and 23 lines height.
// The window width includes two chars for line borders
// The window height includes two for line borders, one for title, and one for command
static struct pp_cell *pokmon[POKWID-2][POKHGT-4] = { 0 };

// headmon is filled with pokmon's via read_pokmons
// headmon is used to place pp_cell's with  pokmon's on pokmon[][]
static struct pokmon *headmon;

// Place some pokmons in the Pokey Play data structure
// Demonstrates how to do this. Not a real game strategy
void create_pokmons() {
    // The variables pm1 and ppc1 demonstrate how to create a pp_cell of type POK_CELL
    // A real game will just use pokmon *headmon to get game pokmons
    static struct pokmon pm1 = { "squagle", 10, 0 };
    static struct pp_cell ppc1 = { POK_CELL, &pm1, 0 };

    headmon = new_pokmon("headpok", 0); // goodness counts pokmons
    pokmon[6][6] = &ppc1;
    pokmon[7][7] = new_pp_cell(POK_CELL, new_pokmon("doodle", 34));
    pokmon[19][6] = &ppc1;
    pokmon[20][6] = &ppc1;
    read_pokmons("pokmons.txt", headmon);
    pokmon[5][5] = new_pp_cell(POK_CELL, get_pokmon(headmon));
    pokmon[4][4] = new_pp_cell(POK_CELL, get_pokmon(headmon));
}

// headfighter is filled with pokmon's via read_pokmons
// headfighter is used to place pp_cell's with fighters's on pokmon[][]
static struct pokmon *headfighter;

// Place some fighters in the Pokey Play data structures
// Demonstrates how to this. Not a real game strategy
void create_fighters() {
    // The variables pm2, ft1, and ppc2 demonstrate how to create a pp_cell of type FIGHT_CELL
    // A real game will just use pokmon *headfighter to get game fighters
    // Notice these variables are not on the stack - it will not work if they are local
    static struct pokmon pm2 = { "baddude", 100, 0 };
    static struct fighter ft1 = { "Gusty", 1, &pm2 };
    static struct pp_cell ppc2 = { FIGHT_CELL, 0, &ft1 };

    pokmon[8][8] = &ppc2;
    headfighter = new_pokmon("headfighter", 0); // goodness counts pokmons
    read_pokmons("fighters.txt", headfighter);
    // The next statement uses the first pokmon in headfighter as the fighter
    // The next statement assumes the headfighter list has at least 2 pokmons on it
    struct fighter *ft = new_fighter(headfighter->next->name, headfighter->next->next);
    pokmon[1][1] = new_pp_cell(FIGHT_CELL, ft);
}

// Define the person playing the game
struct player player = { "Gusty", 10, 0, 0 };

// Places the players Letter on the Pokey Play window
void place_player() {
    px = 5;
    py = 5;
}

// Decrement the player's energy.
// Demonstrates a concept
void decrement_player_energy() {
    player.energy--;
    if (!player.energy)
        printres("You are out of energy");
}

int main(int argc, char **argv) {

    if ( (mainwin = initscr()) == NULL ) {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }
    
    if (LINES < MINLINES || COLS < MINCOLS) {
        fprintf(stderr, "Terminal Size     - LINES: %d, COLS: %d.\n", LINES, COLS);
        fprintf(stderr, "Terminal Minimums - LINES: %d, COLS: %d.\n", MINLINES, MINCOLS);
        delwin(mainwin);
        endwin();
        refresh();
        exit(EXIT_FAILURE);
    }

    /*  Switch of echoing and enable keypad (for arrow keys)  */
    noecho();
    keypad(mainwin, TRUE);
    keypad(stdscr, TRUE);
    mousemask(BUTTON1_PRESSED | BUTTON2_PRESSED, NULL);

    //color_set(1, NULL);
    
    create_pokey_play_win();

    create_results_win();

    printres("Welcome to Pokey Land - Find some Pokmons!");
    printres("Train your Pokmons and fight other Pokmons!");
    updatereswin(reswin);
    wrefresh(reswin);

    create_pokmons();
    create_fighters();
    place_player();

    //wmove(pokwin, 21, 3); // place cursor in pokwin
    int ch = 0;       // ch read from window
    int fighting = 0; // Set to one to indicate fighting
                      // When fighting, you can select a pokmon or p you pokmons
                      // Otherwise fighing is reset
    char command[80]; // collect command entered
    while (1) {
        int poki = 0; // used to put chr read into command[]
        while ( (ch = wgetch(mainwin)) != '\n' ) { // not sure why we need mainwin?
            if (ch == KEY_MOUSE) {
                MEVENT event;
                if (getmouse(&event) == OK) {
                    //printres("Mouse at row=%d, column=%d bstate=0x%08lx", event.y, event.x, event.bstate);
                    // Button presses in the results window
                    if (event.bstate & BUTTON1_PRESSED && event.y > 1 && event.x > 2 && event.y < 15 && event.x < 69) {
                        printres("Mouse-click in Results window");
                        command[0] = 'M';
                        poki = 1;
                    }
                    else if (event.bstate & BUTTON1_PRESSED && event.y > 17 && event.x > 2 && event.y < 37 && event.x < 69) {
                        px = event.x - 2;
                        py = event.y - 18;
                        //printres("Warp Player, px: %d, py: %d", px, py);
                        printres("Warp Player");
                    }
                }
                break;
            }
            else if (ch == KEY_LEFT || ch == KEY_RIGHT ||
                     ch == KEY_UP   || ch == KEY_DOWN) {
                //printres("px: %d, py: %d, poki: %d", px, py, poki);
                decrement_player_energy();
                if (ch == KEY_LEFT) {
                    if (--px < 0)
                        px = 0;
                }
                else if (ch == KEY_RIGHT) {
                    if (++px > 67) // 67 is correct value
                        px = 67;
                }
                else if (ch == KEY_UP) {
                    if (--py < 0)
                        py = 0;
                }
                else if (ch == KEY_DOWN) {
                    if (++py > 18)
                        py = 18;
                }
                if (pokmon[px][py]) {
                    struct pokmon *pm = pokmon[px][py]->pm;
                    struct fighter *ft = pokmon[px][py]->ft;
                    if (pokmon[px][py]->type == POK_CELL) {
                        printres(""); // blank line
                        printres("You found a Pokmon cell! px: %d, py: %d", px, py);
                        printres("Pokmon: %s, Goodness: %d", pm->name, pm->goodness);
                        printres("Enter a g to get the Pokmon into your collection");
                    }
                    else if (pokmon[px][py]->type == FIGHT_CELL) {
                        // Update to select one of the fighter's pokmon
                        // Generate a random # between 0 and ft->num_pokmon-1 to select pokmon
                        // Currently selects the first pokmon on ft->fighters list
                        pm = ft->fighters;
                        printres(""); // blank line
                        printres("You found a Fight cell! px: %d, py: %d", px, py);
                        printres("Fighter: %s, %d, Pokmon: %s, Goodness: %d", ft->name, ft->num_pokmon, pm->name, pm->goodness);
                        printres("Enter an f to fight. Then enter s <pokmon> to select your Pokmon");
                        printres("Enter a p to show your Pokmons");
                    }
                }
                break;
            }
            else {
                command[poki] = ch;
                poki++;
                wechochar(pokwin, ch); // does the wrefresh()
            }
        }
        command[poki] = '\0';

        updatepokwin(pokwin, command);
        // Next two lins clear trailing chars from previous command
        wmove(pokwin, 21, strlen(command)+3); // place cursor in pokwin after command entered
        waddstr(pokwin, "                 "); // a long commands still have trailing characters
        wmove(pokwin, 21, 3); // place cursor in pokwin
        wrefresh(pokwin);
        if (command[0] == 'q')
            break;
        else if (command[0] == 'p') { // show players pokmons
            printres("%s: Your pokmons", player.name);
            // Update to traverse player's linked list of pokmons
            if (player.pm)
                printres("Pokmon: %s, Goodnes: %d", player.pm->name, player.pm->goodness);
            else
                printres("You do not have any Pokmon");
        }
        else if (command[0] == 'g') {
            if (pokmon[px][py] && pokmon[px][py]->type == POK_CELL) {
                // Update to traverse player's link list of pokmons, and add to end
                player.pm = pokmon[px][py]->pm;
                printres("Pokmon added to your collection");
            }
            else
                printres("You are not on a Pokmon");
        }
        else if (command[0] == 'f') {
            if (pokmon[px][py] && pokmon[px][py]->type == FIGHT_CELL) {
                fighting = 1;
                if (player.pm) { // player has some pokmons
                    printres("%s: Select your Pokmon", player.name);
                    // Update to traverse player's linked list of pokmon to find selected
                    printres("Pokmon: %s, Goodness: %d", player.pm->name, player.pm->goodness);
                }
                else {
                    printres("%s: You do not have any Pokmon", player.name);
                }
            }
            else
                printres("You are not on a Fight Cell");
        }
        else if (command[0] == 's' && fighting && pokmon[px][py] && pokmon[px][py]->type == FIGHT_CELL) {
            // Use command[2] as a num or string to find the user's selected pokmon
            fighting = 0;
            char pok_name[20];
            strcpy(pok_name, player.pm->name);
            struct pokmon *player_pm = player.pm;
            // Update to use the game Pokmon selected from HERE
            struct fighter *game_pm = pokmon[px][py]->ft; // put selected pokmon into game_pm
            if (fight(player_pm, game_pm->fighters))
                printres("You won!");
            else {
                printres("%s whooped %s's butt", game_pm->fighters->name, pok_name);
                printres("You lost!");
                player.pm = 0; // remove pokmon from player
            }
        }
        else {
            fighting = 0;
        }
        updatereswin(reswin);
        wrefresh(reswin);
    }


    /*  Clean up after ourselves  */
    delwin(pokwin);
    delwin(reswin);
    delwin(mainwin);
    endwin();
    refresh();

    return EXIT_SUCCESS;
}
