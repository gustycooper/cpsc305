#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "part.h"

part *new_part(char *name, int number, double cost, int count) {
    part *p = malloc(sizeof(part));
    strcpy(p->name, name);
    p->number = number;
    p->cost = cost;
    p->count = count;
    return p;
}

void update_cost(part *p, double percent) {
    p->cost += p->cost * percent;
}

void print_part(part p) {
  printf("Part - name: %10s, number: %7d, cost: $%7.2lf, count%7d\n", p.name, p.number, p.cost, p.count);
}
