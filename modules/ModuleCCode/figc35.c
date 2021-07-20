#include <stdio.h>
#include <stdlib.h>

int main() {
    int *pi = malloc(sizeof(int));
    *pi = 5;
    int i = *pi;
    void *v = &i;
    printf("*pi: %d, pi: %p, i: %d, *v: %d, v: %p\n", 
            *pi,     pi,     i, *(int*)v,   v);
    double *pd = malloc(sizeof(double));
    *pd = 1.4;
    double d = *pd;
    v = pd;
    printf("*pd: %lf, pd: %p, d: %lf, *v: %lf, v: %p\n", 
            *pd,      pd,     d, *(double*)v,  v);
    printf("&pi: %p, &pd: %p\n", &pi, &pd);
}
