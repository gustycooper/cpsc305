#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <ncurses.h>

/****************************************************************************

 Assignment Part 1 - create pokmon module, makefile, and running pokmon

 1. Create a directory pokemon to place your new program in
 2. Create a C module pokmon that has pokmon.h and pokmon.c
    This module is like a data type with values and operations.
    This module is created by removing information from pokey.c

    pokmon.h has the enum, struct, and function prototypes defined in pokey.c

    pokmon.c has the functions.
    struct *pokman new_pokmon(char *name, int goodness) - mallocs memory, fills it in with
    parameters, and returns the pointer
    struct *fighter new_fighter(char *name, struct pokman *pm) - mallocs memory, fills it with
    parameters, and returns the pointer
    struct *pokman fight(struct *pokman p1, struc *pokman p2) - fights the two pokmons and returns
    the winner. This increases goodness of winner and lowers goodness of looser.

 3. Create a C module main.c that has the code that is not removed and placed in module pokmon

    For this main.c, 
    a. update the function create_pokmons() to call new_pokmon()
    b. update the function create_fighters() to call new_fighter()

 4. Create a makefile to build a program called pokemon
    The makefile must use the -lncurses on the linker step.

 5. Run your pokmon to demontsrate it still runs like the original pokey.c

 Assignment Part 2 - Adding to pokmon.h and pokmon.c
 
 1. Add FOOD_CELL to the pp_cell_type

 2. Create a struct food to represent a food cell that can be placed in struct pp_cell *pokmon[POKHGT-4][POKWID-2];

 3. Update pokmon.c to have a struct *food new_food(char *name, int energy)

 4. Update main.c to have a create_food() function that places some food cells on pokmon board

 5. Update main.c to add the energy from food cells to the engery of the player.

 6. Use your makefile to create this new version of pokmon

 7. Test your pokmon to demonstrate these new features work


 Assignment Part 3 - Creating a good fight function

 1. Update main.c to randomly assign various cells to pokmons, fighters, and food.

 2. Create a good fight function. You may need to update the goodness to better reflectc

 3. Use your makefile to create this new version of pokmon

 4. Test your pokmon to demonstrate these new features work

 ****************************************************************************/ 

/****************************************************************************
 This section defines data types used by the program
 ****************************************************************************/ 

enum pp_cell_type { MT_CELL, POK_CELL, FIGHT_CELL };

struct pokman {
    char name[20];
    int goodness;
    struct pokman *next;
};

struct fighter {
    char name[20];
    struct pokman *fighters;
};

struct pp_cell {
    enum pp_cell_type type;
    struct pokman *pm;
    struct fighter  *ft;
};

struct player {
    char name[20];
    int energy;
    int num_pokman;
    struct pokman *pm;
};

/****************************************************************************
 This section defines variables and functions to manipulate the windows
 ****************************************************************************/ 

// Defines for minimal terminal size
#define MINLINES 41
#define MINCOLS 75

// Defines for Results window and Pokey Play window
// Windows have 
// WID - number of chars
// HGT - number of lines
// LIN - start line in window
// COL - start column in window
#define RESWID 70
#define RESHGT 15
#define RESLIN 1
#define RESCOL 1
#define RESDISLINES 12
#define RESVALSLINES 50

#define POKWID 70
#define POKHGT 23
#define POKLIN 16
#define POKCOL 1

// Define our window variables
static WINDOW *mainwin, *pokwin, *reswin;

// Define data structure that mirrors the Pokey Play window
// Pokey Play window is 70 chars wide and 23 lines height.
// The width includes two chars for line borders
// The height includes two for line borders, one for title, and on for command
struct pp_cell *pokmon[POKWID-2][POKHGT-4] = { 0 };

// px and py are the position of the player's letter
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
static void printres(char *fmt, ...) {
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
 This section defines variables and functions to play the game.
 ****************************************************************************/ 
struct pokman pm1 = { "squagle", 10, 0 };
struct pp_cell ppc1 = { POK_CELL, &pm1, 0 };

// Place some pokmons in the Pokey Play data structure
void create_pokmons() {
    pokmon[6][6] = &ppc1;
}

// returns 1 if player wins; otherwise 0
int fight(struct pokman *player, struct pokman *game) {
    // create algorithm to fignt
    return 0;
}

struct pokman pm2 = { "baddude", 100, 0 };
struct fighter ft1 = { "Gusty", &pm2 };
struct pp_cell ppc2 = { FIGHT_CELL, 0, &ft1 };
void create_fighters() {
    pokmon[8][8] = &ppc2;
}

// Places the players Letter on the Pokey Play window
void place_player() {
    px = 5;
    py = 5;
}

struct player player = { "Gusty", 10, 0, 0 };

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
                    if (event.bstate & BUTTON1_PRESSED && event.y > 1 && event.x > 18 && event.y < 15 && event.x < 69) {
                        printres("Mouse-click in Results window");
                        command[0] = 'M';
                        poki = 1;
                    }
                    else if (event.bstate & BUTTON1_PRESSED && event.y > 1 && event.x > 2 && event.y < 37 && event.x < 69) {
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
                if (ch == KEY_LEFT) {
                    if (--px < 0)
                        px = 0;
                    decrement_player_energy();
                }
                else if (ch == KEY_RIGHT) {
                    if (++px > 67) // 67 is correct value
                        px = 67;
                    decrement_player_energy();
                }
                else if (ch == KEY_UP) {
                    if (--py < 0)
                        py = 0;
                    decrement_player_energy();
                }
                else if (ch == KEY_DOWN) {
                    if (++py > 18)
                        py = 18;
                    decrement_player_energy();
                }
                if (pokmon[px][py]) {
                    struct pokman *pm = pokmon[px][py]->pm;
                    struct fighter *ft = pokmon[px][py]->ft;
                    if (pokmon[px][py]->type == POK_CELL) {
                        printres(""); // blank line
                        printres("You found a Pokmon cell! px: %d, py: %d", px, py);
                        printres("Pokmon: %s, Goodness: %d", pm->name, pm->goodness);
                        printres("Enter a g to get the Pokmon into your collection");
                    }
                    else if (pokmon[px][py]->type == FIGHT_CELL) {
                        // Update to select one of the game Pokmon
                        // HERE - selected game Pokmon must be used during fight
                        pm = ft->fighters;
                        printres(""); // blank line
                        printres("You found a Fight cell! px: %d, py: %d", px, py);
                        printres("Fight: %s, Pokmon: %s, Goodness: %d", ft->name, pm->name, pm->goodness);
                        printres("Enter an f to fight. Then enter s <pokmon) to select your Pokmon");
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
            struct pokman *player_pm = player.pm;
            // Update to use the game Pokmon selected from HERE
            struct fighter *game_pm = pokmon[px][py]->ft; // put selected pokman into game_pm
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
