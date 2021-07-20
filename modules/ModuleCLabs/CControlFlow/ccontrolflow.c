#include <stdio.h>

int main(int argc, char **argv) {
    double a[1024], *p = a;
    int aelems = 0;
    while (scanf("%lf", p++) != EOF) // fill a with values from stdin
        aelems++; // count elemenst in a
    // At this point a is filled with aelems
    for (int i = 0; i < aelems; i++)
        printf("a[%d]: %8.2lf\n", i, a[i]);
}
