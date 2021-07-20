#include <stdio.h>
#include "part.h"

// Actual parameter of part p is copied into formal parameter part p
void update_cost1(part p, double percent) {
  // updating actual parameter p does not change the actual parameter
  p.cost += p.cost * percent;
}

// The address in actual parameter of part *p is copied into formal parameter part *p
void update_cost2(part *p, double percent) {
  // updating actual parameter p changes the actual parameter
  p->cost += p->cost * percent;
}

void print_part(part p) {
  printf("Part - name: %10s, number: %7d, cost: $%7.2lf, count%7d\n", p.name, p.number, p.cost, p.count);
}
