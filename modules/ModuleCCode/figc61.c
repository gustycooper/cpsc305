#include <stdio.h>
int main() {
    int a[] = {1,2,3,4,5}, b[5];
    int i, j;
    for (i = 0, j = 4; i < 5; i++, j--)
      b[i] = a[j];
    for (int i = 0; i < 5; i++)
        printf("a[%d]: %d, b[%d]: %d\n", i, a[i], i, b[i]);

}
