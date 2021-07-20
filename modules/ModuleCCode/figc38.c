#include <stdio.h>
int main() {
    int sum = 0, array[] = {1,2,3,4,5};
    for (int i = 0; i < 5; i++)
        sum = sum + array[i];
    printf("sum: %d\n", sum);
    sum = 0;
    for (int *p = array; p <= &array[4]; p++)
        sum = sum + *p;
    printf("sum: %d\n", sum);
}
