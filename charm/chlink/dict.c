#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chlink.h"
#include "dict.h"

extern int verbose;

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

#define DICT_SIZE 31
struct elem *dict[DICT_SIZE] = { 0 };
int num_elems = 0;

struct dict *dictinit() {
    struct dict *d = calloc(1, sizeof(struct dict));
    d->dict = calloc(DICT_SIZE, sizeof(struct elem));
    return d;
}

/*
 dictput - puts key, value pair in symbol table
 input
  struct dict *d - dictionary to update
  char *key - string that is key
  int value - value of key, value pair
 output
  0 - success
  1 - duplicate symbol
 */
int dictput(struct dict *d, const char *key, int value, sections section, char *fn) {
    struct elem *e;
    unsigned long h;
    h = hash_function(key) % DICT_SIZE;
    e = d->dict[h];
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
    e->section = section;
    e->fn = fn;
    e->next = d->dict[h];
    d->dict[h] = e;
    num_elems++;
    /* grow table if there is not enough room
    if(num_elems >= DICT_SIZE * MAX_LOAD_FACTOR) {
        grow(d);
    }
    */
    return 0;
}

/*
 dictupdateone - updates key, value pair in symbol table
 input
  struct dict *d - dictionary to update
  char *key - string that is key
  int value - value of key, value pair
 output
  0 - success
  1 - symbol not in dictionary
 */
int dictupdateone(struct dict *d, const char *key, int value) {
    struct elem *e;
    unsigned long h;
    h = hash_function(key) % DICT_SIZE;
    e = d->dict[h];
    while (e != NULL) {
        if (strcmp(e->key, key) == 0) { // found symbol
            e->value = value;
            return 0;
        }
        e = e->next;
    }
    return 0; // loop did not find value
}

int dictget(struct dict *d, const char *key) {
    for (struct elem *e = d->dict[hash_function(key) % DICT_SIZE]; e != 0; e = e->next) {
        if (!strcmp(e->key, key)) {
            return e->value;
        }
    }
    return -1000001;
}

struct elem *dictgetelem(struct dict *d, const char *key) {
    for (struct elem *e = d->dict[hash_function(key) % DICT_SIZE]; e != 0; e = e->next) {
        if (!strcmp(e->key, key)) {
            return e;
        }
    }
    return 0;
}

static char result[80]; // used to create some verbose outputs

void dictshow(struct dict *d) {
    printf("**** symbols ****\n");
    for (int i = 0; i < DICT_SIZE; i++) {
        for (struct elem *e = d->dict[i]; e != 0; e = e->next) {
            sprintf(result, "symbol: %s, value: 0x%x, section: %d", e->key, e->value, e->section);
            printf("%s\n", result);
        }
    }
}

void dictsymbols(struct dict *d) {
    for (int i = 0; i < DICT_SIZE; i++) {
        for (struct elem *e = d->dict[i]; e != 0; e = e->next) {
            sprintf(result, "%s 0x%x %d", e->key, e->value, e->section);
            printf("%s\n", result);
        }
    }
}

extern struct dict *bigd;

/*
 objinfo have their own dictionaries.
 When a symbol in an object specific dictionary is relocated, the corresponding
 symbol in bigd is also relocated. This allows external symbols to be correctly
 resovled.
 */
void dictrelocate(struct objinfo *o) {
    struct dict *d = o->d;
    for (int i = 0; i < DICT_SIZE; i++) {
        for (struct elem *e = d->dict[i]; e != 0; e = e->next) {
            int startAddr, oldstartAddr;
            if (e->section == DATA) {
                startAddr = o->startDaddr;
                oldstartAddr = o->oldstartDaddr;
            }
            else {
                startAddr = o->startIaddr;
                oldstartAddr = o->oldstartIaddr;
            }
            if (verbose) {
                sprintf(result, "%d, %s 0x%x", e->section, e->key, e->value);
                printf("dictrelocate: %s\n", result);
            }
            e->value = e->value - oldstartAddr + startAddr;
            dictupdateone(bigd, e->key, e->value);
            if (verbose) {
                sprintf(result, "%d, %s 0x%x", e->section, e->key, e->value);
                printf("dictrelocate: %s\n", result);
            }
        }
    }
}

