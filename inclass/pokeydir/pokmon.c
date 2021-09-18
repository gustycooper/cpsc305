#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pokmon.h"

// returns 1 if player wins; otherwise 0
int fight(struct pokmon *player, struct pokmon *game) {
    // create algorithm to fignt
    return 0;
}

struct pokmon *new_pokmon(char *name, int goodness) {
    struct pokmon *p = malloc(sizeof(struct pokmon));
    p->name[NAME_LEN - 1] = '\0'; // put a 0 at last position
    strncpy(p->name, name, NAME_LEN - 1); // cpy upto NAME_LEN-1 chars
    p->goodness = goodness;
    p->next = NULL;
    return p;
}

// headmon.goodness contains num of pokmons in list
// returns 0 on success, -1 on failure
int read_pokmons(char *filename, struct pokmon *head) {
    FILE *f = fopen(filename, "r");
    if (!f)
        return -1;
    char name[NAME_LEN];
    int goodness;
    struct pokmon *prev = head, *p;
    while (fscanf(f, "%s %d", name, &goodness) != EOF) {
        p = new_pokmon(name, goodness);
        head->goodness++;
        prev->next = p;
        prev = p;
    }
    fclose(f);
    return 0;
}

// return a pokmon from the list
// returned pokmon is removed from the list
struct pokmon *get_pokmon(struct pokmon *head) {
    struct pokmon *p = head->next;
    if (p != NULL)
        head->next = p->next;
    return p;
}

// void *pc can be struct pokmon* or struct fighter*
struct pp_cell *new_pp_cell(enum pp_cell_type t, void *pc) {
    struct pp_cell *p = malloc(sizeof(struct pp_cell));
    p->type = t;
    p->pm = NULL; // Ensure pm is NULL
    p->ft = NULL; // Ensure pm is NULL
    if (t == POK_CELL)
        p->pm = pc;
    else if (t == FIGHT_CELL)
        p->ft = pc;
    return p;
}

struct fighter *new_fighter(char *name, struct pokmon *fighters) {
    struct fighter *f = malloc(sizeof(struct fighter));
    f->name[NAME_LEN - 1] = '\0'; // put a 0 at last position
    strncpy(f->name, name, NAME_LEN - 1);
    f->num_pokmon = 0;
    f->fighters = fighters;
    struct pokmon *p = fighters;
    while (p != NULL) {
        f->num_pokmon++;
        p = p->next;
    }
    return f;
}
        

