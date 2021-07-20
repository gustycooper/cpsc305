#include <stdio.h>
#include "part.h"

void print_part(part p) {
  printf("Part - name: %10s, number: %7d, cost: $%7.2lf, count%7d\n", p.name, p.number, p.cost, p.count);
}

extern part global_part;
void print_global_part() {
  print_part(global_part);
}
