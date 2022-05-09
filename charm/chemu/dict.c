#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    int value;
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
int dictput(const char *key, int value) {
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
    e->value = value;
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

void printres(char *fmt, ...) __attribute__((format(printf, 1, 2)));

void dictshow() {
    printres("**** symbols ****");
    for (int i = 0; i < DICT_SIZE; i++) {
        for (struct elem *e = dict[i]; e != 0; e = e->next) {
            printres("symbol: %s, value: 0x%x", e->key, e->value);
        }
    }
}

