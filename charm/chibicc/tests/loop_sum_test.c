int printf(char *format, ...);

int a[] = {1,2,-3,4,5};
int loop_sum() {
    int sum = 0;
    printf("for counting loop");
    for (int i = 0; i < 5; i++) {
        if (a[i] < 0)
            printf("negative: %d", a[i]);
        sum += a[i];
    }
    printf("sum: %d", sum);
    printf("for pointer loop");
    for (int *p = a; p < a+5; p++) {
        if (*p < 0)
            printf("negative: %d", *p);
        sum += *p;
    }
    printf("sum: %d", sum);
    return sum;
}
