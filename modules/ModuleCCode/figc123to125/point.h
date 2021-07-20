#include <stdbool.h>

typedef struct point {
  double x;
  double y;
} point;

point *new_point(double x, double y);
void delete_point(point *p);
void delete_points(int argc, ...);
void print_point(point *p);
double distance(point *p1, point *p2);
bool line(point *p1, point *p2, point *p3);
