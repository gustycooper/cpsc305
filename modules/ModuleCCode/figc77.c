#include <stdio.h>
int main() {
  int array[] = {1,2,3,4,5};
  for (int *ptr = array; ptr < (array + 5); ptr++)
    printf("%d, %d\n", *ptr, ptr[0]);
  for (int i = 0; i < 5; i++)
    printf("%d, %d\n", array[i], *(array+i));
  char s[] = "This is a string.";
  int c = 0;
  for (char *p = s; *p; p++)
    c++;
  printf("length of s: %d\n", c);
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      if (j > i)
        goto outer;
      printf("%d ", i * j);
    }
outer:
  printf("\n");
  }
  return 0;
}
