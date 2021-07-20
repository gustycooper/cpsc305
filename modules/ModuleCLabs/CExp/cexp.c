#include <stdio.h>

/*
Binary Operators from highest to lowest
0. 3 *  multiplication
1. 3 /  division
2. 3 %  remainder
3. 4 +  addition
4. 4 -  subtraction
5. 5 << Bitwise left shift
6. 5 >> Bitwise right shift
7. 6 <  Relational operator less than
8. 6 <= Relational operator less than or equal
9. 6 >  Relational operator greater than
10. 6 >= Relational operator greater than or equal
11. 7 == Relational operator equal
12. 7 != Relational operators not equal
13. 8 &  Bitwise AND
14. 9 ^  Bitwise XOR
15. 10 | Bitwise OR
16. 11 && Logical AND
17. 12 || Logical OR

What operators are not used?
*/

int main(int argc, char **argv) {
    int x, y, i = 0;
    while (scanf("%d %d", &x, &y) != EOF) {
        printf("Line: %2d, x: %2d, y: %2d, ", i+1, x, y);
        printf("%2d +  %2d: %2d, 0x%08x\n", x, y, x + y, x + y);
        i++;
    }
}
