#include <stdio.h>
int main() {
    int x = 5;  // allocates 4 bytes for int x
    int *p;     // allocates 4 bytes for int *p
    p = &x;     // assign the address of x to p
    int y = *p; // dereference p. *p evaluates to the value of x
    // print x, *p, and y - all evaluate to 5 - prints x: 5, *p: 5, y: 5
    printf("x: %d *p: %d y: %d\n", x, *p, y);
    *p = 10;    // *p on left evaluates to the address of x. This assigns 10 to x
    printf("x: %d\n", x); // prints x: 10
    int *q;          // define q to be pointer to int
    q = p;           // assigns 0x200 to q
    int i = *p;      // dereference p to get value of x, assigns 5 to i
    int j = *q + *p; // dereference p and q, assigns 10 to j
    *q = 100;        // dereference q, assigns 100 to x
    printf("x: %d i: %d j: %d\n", x, i, j);
}
