#include <stdio.h>
#include <string.h>
#include "part.h"

part global_part = {"global", 5432, 25}; // global variable - can be accessed outside of main.c
static part static_part; // static variable - can only be accessed inside of main.c

void get_static_part() {
  char name[100];
  int number, count;
  double cost;
  printf("Enter one part:\n");
  printf("Enter part name: ");
  // scanf into local variables
  scanf("%s", name);
  printf("Enter part number: ");
  scanf("%d", &number);
  printf("Enter part cost: ");
  scanf("%lf", &cost);
  printf("Enter part count: ");
  scanf("%d", &count);
  // copy local variables in static_part members
  strcpy(static_part.name, name); // strcpy required for char[]
  static_part.number = number;
  static_part.cost = cost;
  static_part.count = count;
}
int main(int argc, char **argv) {
  // variable global_part can be accessed by functions in main.c and part.c
  global_part.cost += 5;
  print_global_part();
  part local_p = {"bolt", 123, .5};  // declare local_p and assign values
  part local_q;                      // declare local_q - has values left of stack
  print_part(local_p);
  // assign local_p to local_q - all members of p are copied into members of local_q
  local_q = local_p;
  print_part(local_q);
  get_static_part();
  // variable static_part can be accessed by main and get_static_part
  print_part(static_part);

  part a[3]; // declare part array a - has values left on stack
  printf("Enter three parts in loop:\n");
  for (int i = 0; i < 3; i++) {
    printf("Enter part name[%d]: ", i);
    // scanf directly into the array elements
    scanf("%s", a[i].name);
    printf("Enter part number[%d]: ", i);
    scanf("%d", &a[i].number);
    printf("Enter part cost[%d]: ", i);
    scanf("%lf", &a[i].cost);
    printf("Enter part count[%d]: ", i);
    scanf("%d", &a[i].count);
  }
  int i = 0;
  do
    print_part(a[i++]);
  while (i<3);
}
