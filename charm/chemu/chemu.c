#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <ncurses.h>
#include "cpu.h"

/*
TODO 1. Create displayw(int w, char *fmt, ...) to replace
         char temp[80];
         sprintf(temp, "Terminal - LINES: %d, COLS: %d", LINES, COLS);
         addresult(temp);
        Then update code throughout to call displayw
This will use int vsprintf(char *str, const char *format, va_list arg)
The below is from codegen.c of chibicc
__attribute__((format(printf, 1, 2)))
static void println(char *fmt, ...) {
  //printf("in println\n");
  va_list ap;
  va_start(ap, fmt);
  //vfprintf(output_file, fmt, ap);
  vprintf(fmt, ap);
  va_end(ap);
  //fprintf(output_file, "\n");
  printf("\n");
}

TODO 2. Use color to show changes in regs after each step
     3. Mark the PC on Instructions with color - done
     4. Process delete key in commands window - done
     5. Process up / down arrows in commands window - done
     6. Create a scrollable results window - done
TODO 7. Change prompt to indicated waiting and scanf
 */

//#define NCURSES 1

// bg_val used to create white letters/black backgroud or black letters/white background
static int bg_val = 0; // 0 - black, 1 - white, -1 - error

#define MINLINES 41
#define MINCOLS 136

#define REGWID 54
#define REGHGT 14
#define REGLIN 1
#define REGCOL 1

#define INSWID 70
#define INSHGT 14
#define INSLIN 1
#define INSCOL 60

#define CMDWID 54
#define CMDHGT 23
#define CMDLIN 16
#define CMDCOL 1

#define RESWID 70
#define RESHGT 23
#define RESLIN 16
#define RESCOL 60

// Externs from cpu.c used in the register window
// These could be retrieved via getters, get_reg()
extern int registers[16];
extern int cpsr;
extern char mem_changed[80];

/***** Register Window *****/
static const char *regwinl2 = "r0 :%08x r1 :%08x r2 :%08x r3 :%08x";
static const char *regwinl3 = "r4 :%08x r5 :%08x r6 :%08x r7 :%08x";
static const char *regwinl4 = "r8 :%08x r9 :%08x r10:%08x r11:%08x";
static const char *regwinl5 = "r12:%08x r13:%08x r14:%08x r15:%08x";
static const char *regwinl6 = "cpsr:%08x Z: %d, N: %d, C: %d, V: %d, U: %d, OS: %d";
static const char *regwinl8 = "%s";

#ifdef NCURSES
void updateregwin(WINDOW *regwin) {
    mvwprintw(regwin, 2, 1, regwinl2, registers[0],  registers[1],  registers[2],  registers[3]);
    mvwprintw(regwin, 3, 1, regwinl3, registers[4],  registers[5],  registers[6],  registers[7]);
    mvwprintw(regwin, 4, 1, regwinl4, registers[8],  registers[9],  registers[10], registers[11]);
    mvwprintw(regwin, 5, 1, regwinl5, registers[12], registers[13], registers[14], registers[15]);
    mvwprintw(regwin, 6, 1, regwinl6, cpsr, cpsr>>N&1, cpsr>>Z&1, cpsr>>C&1, cpsr>>V&1, cpsr>>U&1, cpsr>>OS&1);
    mvwprintw(regwin, 8, 1, regwinl8, mem_changed);
}
#else
void printregwin() {
    printf(regwinl2, registers[0],  registers[1],  registers[2],  registers[3]);
    printf("\n");
    printf(regwinl3, registers[4],  registers[5],  registers[6],  registers[7]);
    printf("\n");
    printf(regwinl4, registers[8],  registers[9],  registers[10], registers[11]);
    printf("\n");
    printf(regwinl5, registers[12], registers[13], registers[14], registers[15]);
    printf("\n");
    printf(regwinl6, cpsr, cpsr>>N&1, cpsr>>Z&1, cpsr>>C&1, cpsr>>V&1, cpsr>>U&1, cpsr>>OS&1);
    printf("\n");
    printf(regwinl8, mem_changed);
    printf("\n");
}
#endif

/***** Instruction Window *****/
char instinfo[25][80] = {
    {"                                      "},
    {"                                      "},
    {"                                      "},
    {"                                      "},
    {"                                      "},
    {"                                      "},
    {"                                      "},
    {"                                      "},
    {"                                      "},
    {"                                      "},
    {"                                      "},
    {"                                      "},
    {"                                      "},
    {"                                      "},
    {"                                      "},
    {"                                      "},
    {"                                      "},
    {"                                      "},
    {"                                      "},
    {"                                      "},
    };
int insti = 0;
void addinst(char *inst) {
    strcpy(instinfo[insti], "                                                                ");
    strcpy(instinfo[insti], inst);
    insti++;
}
static const char *inswinln  = "%s";

#ifdef NCURSES
void updateinswin(WINDOW *inswin) {
    for (int i = 0; i < 11; i++) {
        mvwprintw(inswin, i+2, 1, inswinln, "                                                                ");
        mvwprintw(inswin, i+2, 1, inswinln, instinfo[i]);
        // Attempt to make the inst pointed to by PC red
        if (i == 5) {
            if (bg_val)
                wcolor_set(inswin, 3, NULL);  // white background
            else
                wcolor_set(inswin, 1, NULL);  // black background
            mvwprintw(inswin, i+2, 1, inswinln, instinfo[i]);
            wrefresh(inswin);
            wcolor_set(inswin, 0, NULL);
        }
        else
            mvwprintw(inswin, i+2, 1, inswinln, instinfo[i]);
        //
    }
    insti = 0;
}
#else
void printinswin() {
    for (int i = 0; i < 11; i++) {
        printf(inswinln, instinfo[i]);
        printf("\n");
    }
    insti = 0;
}
#endif

char cmdhist[25][80] = {
    {"                                      "},
    {"                                      "},
    {"                                      "},
    {"                                      "},
    {"                                      "},
    {"                                      "},
    {"                                      "},
    {"                                      "},
    {"                                      "},
    {"                                      "},
    {"                                      "},
    {"                                      "},
    {"                                      "},
    {"                                      "},
    {"                                      "},
    {"                                      "},
    {"                                      "},
    {"                                      "},
    {"                                      "},
    {"                                      "},
    };

#ifdef NCURSES
// command window is 22 Lines, enumerated as 0 through 21
// command is typed on line 21, which is displayed as a blank line
// Lines 0 through 20 show the previous commands
// Currently command window line numbers are magic numbers
// TODO - Use #define for the command window magic numbers
static const char *cmdwinln = "> %s";
void updatecmdwin(WINDOW *cmdwin, char *command) {
    for (int i = 0; i < 18; i++) {
        strcpy(cmdhist[i], cmdhist[i+1]);
        mvwprintw(cmdwin, i+2, 1, cmdwinln, "                      ");
        mvwprintw(cmdwin, i+2, 1, cmdwinln, cmdhist[i]);
    }
    strcpy(cmdhist[18], "                                      ");
    strcpy(cmdhist[18], command);
    mvwprintw(cmdwin, 20, 1, cmdwinln, "                      ");
    mvwprintw(cmdwin, 20, 1, cmdwinln, cmdhist[18]);
    mvwprintw(cmdwin, 21, 1, cmdwinln, "                      ");
    mvwprintw(cmdwin, 21, 1, cmdwinln, "");
}
#endif
// chemut uses the terminal as its command window

#define RESVALSLINES 50
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
int resi = 0; // count of lines added since last update
int resn = 0; // where to place line in resvals[], circular buffer
void addresult(char *res) {
#ifdef NCURSES
    strcpy(resvals[resn], "                                                                ");
    strcpy(resvals[resn], res);
    resn = (resn + 1) % RESVALSLINES; // place in the resvals buffer
#else
    strcpy(resvals[resi], "                                                                ");
    strcpy(resvals[resi], res);
#endif
    resi++;
}

// printres has variable arguments just like fprintf
__attribute__((format(printf, 1, 2)))
void printres(char *fmt, ...) {
    char res[80];
    va_list ap;
    va_start(ap, fmt);
    vsprintf(res, fmt, ap);
    va_end(ap);
    addresult(res);
}

static const char *reswinln = "%s";

#ifdef NCURSES
#define RESDISLINES 20
int scrn = 0; // where to begin scrolling
void updatereswin(WINDOW *reswin) {
    int disi = resn -  RESDISLINES; // resvals[disi] is start of display
    if (disi < 0) // wrap around resvals[]
        disi = RESVALSLINES + disi;
    for (int i = 0; i < RESDISLINES; i++) {
        mvwprintw(reswin, i+2, 1, reswinln, "                                                           ");
        mvwprintw(reswin, i+2, 1, reswinln, resvals[disi]);
        disi = (disi + 1) % RESVALSLINES;
    }
    resi = 0;
    scrn = resn; // establish scroll position
}
void scrollreswin(WINDOW *reswin) {
    int disi = scrn -  RESDISLINES; // resvals[disi] is start of display
    if (disi < 0) // wrap around resvals[]
        disi = RESVALSLINES + disi;
    for (int i = 0; i < RESDISLINES; i++) {
        mvwprintw(reswin, i+2, 1, reswinln, "                                                           ");
        mvwprintw(reswin, i+2, 1, reswinln, resvals[disi]);
        disi = (disi + 1) % RESVALSLINES;
    }
}
#else
void printreswin() {
    for (int i = 0; i < resi; i++) {
        printf(reswinln, resvals[i]);
        printf("\n");
    }
    resi = 0;
}
#endif


extern int doscanf;

int cmdgetstr(char **ps, char *es, char **str);
#define MAXARGS 10
static char *cmdargv[MAXARGS];
int do_cmd(int argc, char **cmdargv);
void do_script(char *scriptfilename);
int process_args(int argc, char **argv);
int getcmd(char *buf, int nbuf);

#define BRIGHT_WHITE 15

int main(int argc, char **argv) {
    bg_val = process_args(argc, argv); // 0 - black, 1 - white, -1 - error
    if (bg_val == -1)
        exit(EXIT_FAILURE);
#ifdef NCURSES
    WINDOW *mainwin, *regwin, *inswin, *cmdwin, *reswin;
    int ch;

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
    start_color();
    if (has_colors()) {
        init_pair(1, COLOR_RED, COLOR_BLACK);
        init_pair(2, COLOR_WHITE, COLOR_BLACK);
        if (bg_val == 1) { // white background
            if (can_change_color()) {
                init_color(BRIGHT_WHITE, 1000,1000,1000);
                init_pair(0xff, COLOR_BLACK, BRIGHT_WHITE);
                init_pair(3, COLOR_RED, BRIGHT_WHITE);
            } 
            else if (COLORS >= 16) {
                init_pair(0xff, COLOR_BLACK, BRIGHT_WHITE);
                init_pair(3, COLOR_RED, BRIGHT_WHITE);
            } 
            else {
                init_pair(0xff, COLOR_BLACK, COLOR_WHITE);
                init_pair(3, COLOR_RED, COLOR_WHITE);
            }
            wbkgd(mainwin, COLOR_PAIR(0xff));
        }
        //wattron(inswin, COLOR_PAIR(1));
    }
        

    /*  Switch of echoing and enable keypad (for arrow keys)  */
    noecho();
    keypad(mainwin, TRUE);
    keypad(stdscr, TRUE);
    mousemask(BUTTON1_PRESSED | BUTTON2_PRESSED, NULL);


    /*  Create register window */
    regwin = subwin(mainwin, REGHGT, REGWID, REGLIN, REGCOL);
    box(regwin, 0, 0);
    wattron(regwin, A_BOLD);
    mvwaddstr(regwin, 1, 1, "                     Registers                     ");
    mvwaddstr(regwin, 2, 1, "r0 :00000000 r1 :00000000 r2 :00000000 r3 :00000000");
    mvwaddstr(regwin, 3, 1, "r4 :00000000 r5 :00000000 r6 :00000000 r7 :00000000");
    mvwaddstr(regwin, 4, 1, "r8 :00000000 r9 :00000000 r10:00000000 r11:00000000");
    mvwaddstr(regwin, 5, 1, "r12:00000000 r13:00000000 r14:00000000 r15:00000000");
    mvwaddstr(regwin, 6, 1, "cpsr:00000000 Z: 0, N: 0, C: 0, V: 0, U: 0, OS: 0  ");
    mvwaddstr(regwin, 7, 1, "                   Memory Changed                  ");
    mvwaddstr(regwin, 8, 1, "addr: 0xffff, before: 0xffffffff, after: 0xffffffff");
    updateregwin(regwin);

    /*  Create instruction window */
    inswin = subwin(mainwin, INSHGT, INSWID, INSLIN, INSCOL);
    box(inswin, 0, 0);
    wattron(inswin, A_BOLD);
    mvwaddstr(inswin, 1, 1, "                           Instructions                  ");
    updateinswin(inswin);
    // colors are not used yet. Code remains as a reminder
    //color_set(1, NULL);

    
    /*  Create command window */
    cmdwin = subwin(mainwin, CMDHGT, CMDWID, CMDLIN, CMDCOL);
    box(cmdwin, 0, 0);
    wattron(cmdwin, A_BOLD);
    mvwaddstr(cmdwin, 1, 1, "                     Commands                      ");
    updatecmdwin(cmdwin, "start");

    printres("Terminal - LINES: %d, COLS: %d", LINES, COLS);

    /*  Create result window */
    reswin = subwin(mainwin, RESHGT, RESWID, RESLIN, RESCOL);
    box(reswin, 0, 0);
    wattron(reswin, A_BOLD);
    mvwaddstr(reswin, 1, 1, "                             Results                      ");
    updatereswin(reswin);
#else
    printreswin();
#endif

    char command[80], command_copy[80];
    while (1) {
#ifdef NCURSES
        int cmdi = 0; // command index
        int hisi = 18; // starting point of command history
        int xf, yf;
        int arrow = 0; // no arrow key
        while ( (ch = wgetch(mainwin)) != '\n' ) {
            getyx(cmdwin, yf, xf);
            if (ch == KEY_BACKSPACE || ch == KEY_DC || ch == 127) {
                if (xf - 1 < 3)
                    xf = 3;
                else
                    xf -= 1;
                mvwdelch(cmdwin, yf, xf);
                wrefresh(cmdwin);
                cmdi--;
                if (cmdi < 0)
                    cmdi = 0;
            }
            else if (ch == KEY_MOUSE) {
                MEVENT event;
                char buffer[80];
                if (getmouse(&event) == OK) {
                    //snprintf(buffer, 80, "Mouse at row=%d, column=%d bstate=0x%08lx",
                    //         event.y, event.x, event.bstate);
                    //addresult(buffer);
                    if (event.bstate & BUTTON1_PRESSED && event.y > 1 && event.x > 60 && event.y < 13 && event.x < 126) {
                        snprintf(buffer, 80, "Step by mouse-click in Instructions window");
                        addresult(buffer);
                        command[0] = command_copy[0] = 's';
                        cmdi = 1;
                    }
                }
                break;
            }
            else if (ch == KEY_LEFT || ch == KEY_RIGHT) { // scroll the results window
                arrow = 1;
                if (ch == KEY_LEFT) {
                    scrn--;
                    if (scrn < 0)
                        scrn = RESVALSLINES - 1;
                }
                else {
                    scrn = (scrn + 1) % RESVALSLINES;
                }
                break;
            }
            else if (ch == KEY_UP || ch == KEY_DOWN) { // retrieve command history
                strcpy(command, cmdhist[hisi]);
                strcpy(command_copy, cmdhist[hisi]);
                cmdi = strlen(command);
                mvwprintw(cmdwin, 21, 1, cmdwinln, "                      ");
                mvwprintw(cmdwin, 21, 1, cmdwinln, command_copy);
                wrefresh(cmdwin);
                if (ch == KEY_UP) {
                    hisi--;
                    if (hisi < 0)
                        hisi = 0;
                }
                else {
                    hisi++;
                    if (hisi > 18)
                        hisi = 18;
                }
            }
            else {
                command[cmdi] = ch;
                command_copy[cmdi] = ch;
                cmdi++;
                hisi = 18;
                wechochar(cmdwin, ch); // does the wrefresh()
            }
        }
        command[cmdi] = '\0';
        command_copy[cmdi] = '\0';
#else
        int status = getcmd(command, sizeof(command));
        if (status < 0 || command[0] == 'q')
            break;
        strcpy(command_copy, command);
#endif
        if (doscanf) {
            chemuscanf(command);
            goto skipcommand;
        }
        char *s = command;
        char *es = s + strlen(s);
        char *str;
        int argc = 0;
        cmdargv[0] = ""; // prevent seg fault when first cmd is empty line
        while (cmdgetstr(&s, es, &str) != 0) {
            cmdargv[argc] = str;
            //printf("%s\n", cmdargv[argc]);
            argc++;
        }
#ifdef NCURSES
        if (!arrow)
            if (strlen(command_copy))
                addresult(command_copy);
#endif
        cmdargv[argc] = 0;
        if (argc > 0) {
            if (cmdargv[0][0] != '<') {
                if (do_cmd(argc, cmdargv) == 0)
                    break;
            }
            else {
              do_script(cmdargv[1]);
            }
        }
skipcommand:
#ifdef NCURSES
        updateregwin(regwin);
        updateinswin(inswin);
        if (arrow)
            scrollreswin(reswin);
        else {
            updatereswin(reswin);
            if (strlen(command_copy))
                updatecmdwin(cmdwin, command_copy);
        }
        wrefresh(regwin);
        wrefresh(inswin);
        wrefresh(reswin);
        wmove(cmdwin, 21, 3);
        wrefresh(cmdwin);
#else
        printreswin();
        if (command[0] == 's') { // some form of step
            printregwin();
            printinswin();
        }
        else if (command[0] == 'r')
            printregwin();
        else if (command[0] == 'p' && command[1] == 'l')
            printinswin();
        // chemut accepts <space>s, which calls pipeline, but does not call printinstwin() to reset insti
        // Two <space>s commands in a row will seg fault because insti gets too big.
        // Reset insti to prevent seg fault
        insti = 0;
#endif
        if (command_copy[0] == 'q')
            break;
    }


#ifdef NCURSES
    /*  Clean up after ourselves  */
    delwin(regwin);
    delwin(inswin);
    delwin(cmdwin);
    delwin(reswin);
    delwin(mainwin);
    endwin();
    refresh();
#endif

    return EXIT_SUCCESS;
}
