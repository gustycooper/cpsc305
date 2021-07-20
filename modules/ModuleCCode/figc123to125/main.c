#include <stdio.h>
#include "point.h"

int main() {
  point *p1 = new_point(0,1);
  point *p2 = new_point(0,0);
  printf("distance((0,1),(0,0)): %lf\n", distance(p1, p2));
  point *p3 = new_point(0,8);
  printf("line((0,1),(0,0),(0,8)): %d\n", line(p1, p2, p3));
  p1->x = 4;
  p1->y = 6;
  print_point(p1);
  printf("distance((0,1),(4,6)): %lf\n", distance(p1, p2));
  printf("line((0,1),(0,0),(4,6)): %d\n", line(p1, p2, p3));
  delete_points(3, p1, p2, p3);
  return 0;
}
