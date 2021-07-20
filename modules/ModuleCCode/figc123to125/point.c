#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include "point.h"

#define DEBUG 1

point *new_point(double x, double y) {
  point *p = malloc(sizeof(point));
  p->x = x;
  p->y = y;
  return p;
}

void delete_point(point *p) {
  free(p);
}

void delete_points(int argc, ...) { 
  va_list ap; 
  va_start(ap, argc); 
  for (int i = 1; i <= argc; i++) {
     void *p = va_arg(ap, void *);
     #if DEBUG
     printf("Freeing p->x: %lf, p->y: %lf\n", ((point*)p)->x, ((point*)p)->y);
     #endif
     free(p);
  }
  va_end(ap); 
} 

void print_point(point *p) {
  printf("(%.2lf, %.2lf)\n", p->x, p->y);
}

double distance(point *p1, point *p2) {
  return sqrt(pow(p1->x - p2->x, 2) + pow(p1->y - p2->y, 2));
}

// see https://www.geeksforgeeks.org/program-check-three-points-collinear/
bool line(point *p1, point *p2, point *p3) {
   return (p3->y - p2->y) * (p2->x - p1->x) == (p2->y - p1->y) * (p3->x - p2->x);
 //return (y3    - y2)    * (x2    - x1)    == (y2    - y1) *    (x3    - x2) 
}
