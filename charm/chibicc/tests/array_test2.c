int printf(char *format, ...);
int a[] = {1,2,3};
int a1 = 5;
int this = 100;
int f() {
    int b[] = {4,5,6};
    for (int *p = a; p < a+3; p++)
        this += *p;
    for (int i = 0; i < 3; i++)
        a1 += b[i];
    printf("Gusty: %d\n", a1);
}
