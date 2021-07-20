#include <stdio.h>
int main() {
    int array[5] = {1,2,3,4,5};
    int *pa1, *pa2;
    pa1 = array;
    pa2 = &array[0];
    int i = array[2];
    printf("i: %d\n", i);
    i = pa1[2];
    printf("i: %d\n", i);
    i = *(pa2 + 2);
    printf("i: %d\n", i);
    i = *(array + 2);
    printf("i: %d\n", i);
}
