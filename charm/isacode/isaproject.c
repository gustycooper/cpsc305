#include <stdio.h>

void sort(int *ia, int s) {
    for (int i = 0; i < s; i++)
        for (int j = i+1; j < s; j++)
            if (ia[i] > ia[j]) {
                int t = ia[j];
                ia[j] = ia[i];
                ia[i] = t;
            }
}

int count(int *ia, int s, int v) {
    int c = 0;
    for (int i = 0; i < s; i++)
        if (ia[i] == v)
            c++;
    return c;
}
        
int largest(int *ia, int s) {
    int l = *ia;
    for (int *p = ia; p < ia+s; p++)
        if (*p > l)
            l = *p;
    return l;
}

int numelems(int *ia) {
    int c = 0;
    while (*ia++ != 0)
        c++;
    return c;
}

static int staticia[] = {50,43,100,-5,-10,50,0};
int main() {
    int ia[] = {2,3,5,1,0};
    sort(ia, numelems(ia));
    for (int i = 0; i < numelems(ia); i++)
        printf("ia[%d]: %d\n", i, ia[i]);
    sort(staticia, numelems(staticia));
    for (int i = 0; i < numelems(staticia); i++)
        printf("ia[%d]: %d\n", i, staticia[i]);
    printf("ia:       count of %d is %d\n", 50, count(ia, numelems(ia), 50));
    printf("staticia: count of %d is %d\n", 50, count(staticia, numelems(staticia), 50));
    printf("largest: %d\n", largest(ia, numelems(ia)));
    printf("largest: %d\n", largest(staticia, numelems(staticia)));
    if (largest(ia, numelems(ia)) != ia[numelems(ia)-1])
        printf("Something bad\n");
    else
        printf("Nice sort and largest\n");
    if (largest(staticia, numelems(staticia)) != staticia[numelems(staticia)-1])
        printf("Something bad\n");
    else
        printf("Nice sort and largest\n");
}
