#include <stdio.h>

int main(int argc, char **argv) {
    int a = 40, b = 25, c = 0;
    int ta, tb;
    printf("Enter a: ");
    scanf("%d", &ta);
    printf("Enter b: ");
    scanf("%d", &tb);
    a = ta;
    b = tb;
    if (a <= b) {
        a -= b;
        printf("a <= b, a: %d\n", a);
    }
    else {
        b += a;
        printf("a > b, b: %d\n", b);
    }
    c = a + b;
    printf("c = a + b, c: %d, a: %d, b: %d\n", c, a, b);

    a = ta;
    b = tb;
    if (a > b)
        goto else_part;
        a -= b;
        printf("a <= b, a: %d\n", a);
        goto end_if;
    else_part:
        b += a;
        printf("a > b, b: %d\n", b);
    end_if:
    c = a + b;
    printf("c = a + b, c: %d, a: %d, b: %d\n", c, a, b);
}
