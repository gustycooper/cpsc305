#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// global and static variables - in static section
int gi = 77;
int *gpi = &gi;
static double gd = 8.55;
static double *gpd = &gd;

int main(int argc, char *argv[]) {
    // variables on the stack
    int i = 7;
    int *pi = &i;
    int *pj = malloc(sizeof(int)); // allocate mem on heap
    *pj = 88;
    double d = 4.25;
    double *pd = &d;
    // notice c3 = 2 and c4 = 100
    char c1 = 'a', c2 = 'b', c3 = 2, c4 = 100;
    c3 += c1; // char is a 1-byte integer
    int ia[] = {1,2,3,4,5};
    double da[] = {2,4,6,8,10};
    // Is the same string "Gusty" in both ca and cp?
    char ca[] = "Gusty";
    char *cp = "Gusty";

    printf("Display variables and attributes\n");
    printf("type\tname\tvalue\t\taddr\t\tnum bytes\n");
    printf("int\tgi\t%d\t\t%p\t%ld\n", gi,  &gi, sizeof(gi));
    printf("int*\tgpi\t%p\t%p\t%ld\n", gpi,  &gpi, sizeof(gpi));
    printf("int\ti\t%d\t\t%p\t%ld\n", i,  &i, sizeof(i));
    printf("int*\tpi\t%p\t%p\t%ld\n", pi,  &pi, sizeof(pi));
    printf("int\t*pi\t%d\t\t%p\t%ld\n", *pi,  pi, sizeof(*pi));
    printf("int*\tpj\t%p\t%p\t%ld\n", pj,  &pj, sizeof(pj));
    printf("int\t*pj\t%d\t\t%p\t%ld\n", *pj,  pj, sizeof(*pj));
    printf("double\tgd\t%4.2lf\t\t%p\t%ld\n",  gd,  &gd, sizeof(gd));
    printf("double*\tgpd\t%p\t%p\t%ld\n", gpd,  &gpd, sizeof(gpd));
    printf("double\td\t%4.2lf\t\t%p\t%ld\n",  d,  &d, sizeof(d));
    printf("double*\tpd\t%p\t%p\t%ld\n", pd,  &pd, sizeof(pd));
    printf("int\t*pd\t%4.2lf\t\t%p\t%ld\n", *pd,  pd, sizeof(*pd));
    printf("type\tname\tvalue\t\taddr\t\tnum bytes\n");
    printf("\t\tchar\tdec\n");
    printf("char\tc1\t%c\t%d\t%p\t%ld\n", c1, c1, &c1, sizeof(c1));
    printf("char\tc2\t%c\t%d\t%p\t%ld\n", c2, c2, &c2, sizeof(c2));
    printf("char\tc3\t%c\t%d\t%p\t%ld\n", c3, c3, &c3, sizeof(c3));
    printf("char\tc4\t%c\t%d\t%p\t%ld\n", c4, c4, &c4, sizeof(c4));
    printf("\nIterate through ia using a counting for loop.\n");
    printf("type\tname\tvalue\t\taddr\t\tnum bytes\n");
    for (int i = 0; i < 5; i++)
        printf("int\tia[%d]\t%d\t\t%p\t%ld\n", i, ia[i], &ia[i], sizeof(int));

    printf("\nIterate through ia using a pointer arithmetic for loop.\n");
    printf("type\tname\tvalue\t\taddr\t\tnum bytes\n");
    i = 0;
    for (int *p = ia; p < ia+5; p++)
        printf("int\tia[%d]\t%d\t\t%p\t%ld\n", i++, *p, p, sizeof(int));

    printf("\nIterate through da using a counting for loop.\n");
    printf("type\tname\tvalue\t\taddr\t\tnum bytes\n");
    for (int i = 0; i < 5; i++)
        printf("int\tda[%d]\t%4.2lf\t\t%p\t%ld\n", i, da[i], &da[i], sizeof(double));

    printf("\nIterate through ca using a counting for loop.\n");
    printf("type\tname\tvalue\t\taddr\t\tnum bytes\n");
    printf("\t\tchar\tdec\n");
    for (int i = 0; i <= strlen(ca); i++)
        printf("int\tca[%d]\t%c\t%02d\t%p\t%ld\n", i, ca[i], ca[i], &ca[i], sizeof(char));

    printf("\nIterate through cp using a counting for loop.\n");
    printf("type\tname\tvalue\t\taddr\t\tnum bytes\n");
    printf("\t\tchar\tdec\n");
    for (int i = 0; i <= strlen(cp); i++)
        printf("int\tcp[%d]\t%c\t%02d\t%p\t%ld\n", i, cp[i], cp[i], &cp[i], sizeof(char));

    return 0;
}

