#include <stdio.h>
#include <stdlib.h>

int g = 5;
static int s = 6;
int main() {
    int l = 7;
    int *p = malloc(sizeof(int));
    printf("code: %p\n", main);
    printf("static: %p, %p\n", &g, &s);
    printf("heap: %p\n", p);
    printf("stack: %p\n", &l);
}
