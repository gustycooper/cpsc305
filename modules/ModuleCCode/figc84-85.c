#include <stdio.h>

int global_a[] = {1,2,3,4,5,6,7};
static int static_a[] = {1,2,3,4,5,6,7};

static double avg(int *a, int len) {
    double sum = 0;
    for (int i = 0; i < len; i++)
      sum += a[i];
    return sum / len;
}

void my_print(int a[], int len, char name[]);

int main() {
    int local_a[] = {1,2,3,4,5};
    my_print(local_a, 5, "local_a");
    my_print(global_a, 7, "global_a");
    my_print(static_a, 7, "static_a");
    return 0;
}

double sum(int a[], int len) {
    double sum = 0;
    for (int *p = a; p < a + len; sum+=*p, p++);
    return sum;
}

void my_print(int *a, int len, char *name) {
    printf("%s  avg: %lf, sum: %lf\n", name, avg(a, len), sum(a, len));
}
