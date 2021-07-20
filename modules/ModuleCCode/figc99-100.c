#include <stdio.h>
int main() {
    int a, b, *p = &b;						
    printf("Enter two numbers: ");
    scanf("%d %d", &a, p);
    printf("%d, %d\n",  a, b);
    int m = 0, d = 0, y = 0;
    printf("Enter a date: ");
    int c = scanf("%d/%d/%d", &m, &d, &y);
    printf("c: %d, m/d/y: %d/%d/%d\n", c, m, d, y);
    return 0;
}
