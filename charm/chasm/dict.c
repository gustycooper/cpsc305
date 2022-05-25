#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chasm_types.h"
#include "dict.h"

// Dictionary implementation - simple version
// See https://www.cs.yale.edu/homes/aspnes/pinewiki/C(2f)HashTables.html?highlight=%28CategoryAlgorithmNotes%29
// for a more complex version
#define MULTIPLIER (37) // nice little prime number
unsigned long hash_function(const char *s) {
    unsigned long h;
    unsigned const char *us = (unsigned const char *)s; // use values >= 0
    h = 0;
    while(*us != '\0') {
        h = h * MULTIPLIER + *us;
        us++;
    } 
    return h;
}

struct elem {
    struct elem *next;
    char *key;
    enum dicttype type;
    int value;
    char *svalue;
    sections section;
};
#define DICT_SIZE 31
struct elem *dict[DICT_SIZE] = { 0 };
int num_elems = 0;

/*
 dictput - puts key, value pair in symbol table
 input
  char *key - string that is key
  int value - value of key, value pair
 output
  0 - success
  1 - duplicate symbol
 */
int dictput(const char *key, int value, sections section) {
    struct elem *e;
    unsigned long h;
    h = hash_function(key) % DICT_SIZE;
    e = dict[h];
    while (e != NULL) {
        if (strcmp(e->key, key) == 0) {
            //printf("duplicate symbol: %s", key);
            return 1;
        }
        e = e->next;
    }
    e = malloc(sizeof(*e));
    e->key = strdup(key);
    e->type = INT;
    e->value = value;
    e->svalue = 0;
    e->section = section;
    e->next = dict[h];
    dict[h] = e;
    num_elems++;
    /* grow table if there is not enough room
    if(num_elems >= DICT_SIZE * MAX_LOAD_FACTOR) {
        grow(d);
    }
    */
    return 0;
}

int dictputval(struct dictval *dv) {
    char *key = dv->key;
    struct elem *e;
    unsigned long h;
    h = hash_function(key) % DICT_SIZE;
    e = dict[h];
    while (e != NULL) {
        if (strcmp(e->key, key) == 0) {
            //printf("duplicate symbol: %s", key);
            return 1;
        }
        e = e->next;
    }
    e = malloc(sizeof(*e));
    e->key = strdup(key);
    e->type = dv->type;
    e->value = dv->ivalue;
    e->svalue = dv->svalue;
    e->section = dv->section;
    e->next = dict[h];
    dict[h] = e;
    num_elems++;
    /* grow table if there is not enough room
    if(num_elems >= DICT_SIZE * MAX_LOAD_FACTOR) {
        grow(d);
    }
    */
    return 0;
}

int dictget(const char *key) {
    for (struct elem *e = dict[hash_function(key) % DICT_SIZE]; e != 0; e = e->next) {
        if (!strcmp(e->key, key)) {
            return e->value;
        }
    }
    return -1000001;
}

int dictgetval(const char *key, struct dictval *dv) {
    for (struct elem *e = dict[hash_function(key) % DICT_SIZE]; e != 0; e = e->next) {
        if (!strcmp(e->key, key)) {
            dv->key = strdup(key);
            dv->type = e->type;
            dv->ivalue = e->value;
            if (e->svalue)
                dv->svalue = strdup(e->svalue);
            else
                dv->svalue = 0;
            dv->section = e->section;
            return 0;
        }
    }
    return 1;
}

void dictprint(int verbose) {
    if (verbose)
        printf("**** dict contents ****\n");
    int totelems = 0, totcols = 0;
    for (int i = 0; i < DICT_SIZE; i++) {
        int col = 0;
        for (struct elem *e = dict[i]; e != 0; e = e->next) {
            if (e->type == INT) { // Do not print extern symbols
                if (verbose)
                    printf("key: %s, value: %d section: %d\n", e->key, e->value, e->section);
                else
                    printf("%s 0x%x %d\n", e->key, e->value, e->section);
                totelems++;
                col++;
            }
        }
        if (col > 1)
            totcols = totcols + (col - 1);
    }
    if (verbose)
        printf("totelems: %d, totcols: %d\n", totelems, totcols);
}

