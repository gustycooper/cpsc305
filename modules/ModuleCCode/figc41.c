#include <stdio.h>
int main() {
    int x = 5, a = 10;
    a = a + x++; // after this a is 15 and x is 6
    printf("a: %d, x: %d\n", a, x);
    x = 5; a = 10;
    a = a + ++x; // after this a is 16 and x is 6
    printf("a: %d, x: %d\n", a, x);
}
