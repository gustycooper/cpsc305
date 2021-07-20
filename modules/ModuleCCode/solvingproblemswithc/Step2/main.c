#include <stdio.h>
#include <string.h>
#include "part.h"

int main(int argc, char **argv) {
    // p is type part. p has enough memory to hold a part
    part p = {"bolt", 123, .5, 5};
    // q is type part* (pointer to part). q has enough memory to hold an address
    part *q;
    // The expresson &p evaluates to type part*
    q = &p; // q points to p
    // The expression *q evaluates to type part
    (*q).cost = 6;  // *q and p both access the same part
    p.number = 432; // changing p and *q change the same memory
    q->count = 41;
    printf("print_part(p): ");
    print_part(p); // our p has changed number and cost
    // assign *q to r - all members of p are copied into members of r
    part r = *q;
    printf("print_part(r): ");
    print_part(r);


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
    strcpy(p.name, name);
    p.number = number;
    q->cost = cost;
    p.count = count;
    printf("print_part(*q): ");
    print_part(*q);

    update_cost1(p, .1); // udpate_cost1 does not change p
    printf("print_part(p) : ");
    print_part(p);       // equivalent to print_part(*q)
    update_cost2(q, .1); // update_cost2 changes p, equivalent to udpate_cost2(&p, .1)
    printf("print_part(p) : ");
    print_part(p);

}
