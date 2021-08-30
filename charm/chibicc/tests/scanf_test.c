int printf(char *format, ...);
int scanf(char *format, ...);
int b = 0xaabbccdd;
int f() {
    printf("Enter int: ");
    scanf("%d", &b);
    printf("Entered: %d", b);
}
