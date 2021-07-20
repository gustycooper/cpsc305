#include <stdio.h>

int main() {
  int i = 2;
  int j;
  double d = 3.14159;
  double e;
  int a, b = i * 4, c = 0;
  double x, y = d * 2, z = 0;
  int ia1[] = {1,2,3}, ia2[5];
  double da1[] = {1.1,2.2,3.3}, da2[5];
  char sa1[] = "Gusty", sa2[5];
  j = (int)d;
  a = 0x10;
  e = b;
  x = 5.0;
  da2[3] = 3.3;
  sa2[0] = 'O';
  sa2[1] = 'k';
  sa2[2] = '\0';
  printf("%d, %d, %d, %d, %d\n", i, j, a, b, c);
  printf("%lf, %lf, %lf, %lf, %lf\n", d, e, x, y, z);
  printf("%d, %d, %d\n", ia1[0], ia1[1], ia1[2]);
  printf("%lf, %lf, %lf\n", da1[0], da1[1], da1[2]);
  printf("sa1: %s, sa2: %s\n", sa1, sa2);
}
