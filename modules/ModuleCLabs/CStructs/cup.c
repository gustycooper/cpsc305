#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cup.h"

char *contents_to_str[] = {"TEA", "COFFEE", "WATER", "MILK", "MIXED"};
char *material_to_str[] = {"PAPER", "GLASS", "PLASTIC"};

void print_cup(cup *c) {
    printf("%s %d %d %s %s\n", c->color, c->max_oz, c->cur_oz, 
           contents_to_str[c->contents], material_to_str[c->material]);
}

cup *new_cup(int max, int cur, contents contents, enum material m, char *color) {
    cup *p = malloc(sizeof(cup));
    p->max_oz = max;
    p->cur_oz = cur;
    p->contents = contents;
    p->material = m;
    strcpy(p->color, color);
    return p;
}

int fill(cup *c, contents v, int oz) {
    c->cur_oz += oz;
    if (c->contents != v)
        c->contents = MIXED;
    if (c->cur_oz > c->max_oz) {
        c->cur_oz = c->max_oz;
        return -1;
    }
    return c->cur_oz;
}

cup *find_fullest(cup **ca, int num_eles) {
    return 0;
}

void sort_cups(cup **ca, int num_eles) {
    return;
}

void print_cups(cup **ca, int num_eles) {
    for (int i = 0; i < num_eles; i++) {
        printf("cup[%d]: ", i);
        print_cup(ca[i]);
    }
}

