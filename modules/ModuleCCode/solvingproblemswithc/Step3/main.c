#include <stdio.h>
#include <string.h>
#include "part.h"

int main(int argc, char **argv) {
    // p is type part. p has enough memory to hold a part
    part p = {"bolt", 123, .5, 4};
    // q is type part* (pointer to part). q has enough memory to hold an address
    part *q = new_part("screw", 345, 6, 10);
    printf("print_part(p) : ");
    print_part(p); // our p has changed number and cost
    printf("print_part(*q): ");
    print_part(*q);


    // name is a char array of 100 chars
    char name[100];
    // number is type int, n is type int*
    int number, *n, count;
    // cost is type double, c is type double*
    double cost, *c;
    n = &number;
    c = &cost;
    // scanf arguments must be pointers - scanf reads from the terminal into address
    printf("Enter part name: ");
    scanf("%s", name);
    printf("Enter part number: ");
    scanf("%d", n);  // equivalent to scanf("%d", &number);
    printf("Enter part cost: ");
    scanf("%lf", c);  // equivalent to scanf("%lf", &cost);
    printf("Enter part count: ");
    scanf("%d", &count);
    q = new_part(name, number, cost, count);
    printf("print_part(*q): ");
    print_part(*q);

    // C arrays and pointers are somewhat interchangeable
    part pa[3] = {{"A", 1, 10, 100}, {"B", 2, 20, 15}, {"C", 3, 30, 2}};
    part *pap;
    pap = pa; // pa is equivalent to &pa[0]
    printf("\nprint_part(pa[1]):    ");
    print_part(pa[1]);
    printf("print_part(pap[1]):   ");
    print_part(pap[1]);
    printf("print_part(*(pap+1)): ");
    print_part(*(pap+1));


}
