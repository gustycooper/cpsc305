#include <stdio.h>
int global_int = 5;
static int static_int = 6;
int main() {
  int local_int = 7;
  printf("%d, %d, %d\n", global_int, static_int, local_int);
}
