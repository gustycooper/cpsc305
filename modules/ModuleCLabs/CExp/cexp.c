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
/*
Add 11 printf statements that print the answers to each of the following questions
about expressions. Compute the answer using your brain, and print the answer in 
decimal and hex as follows, which prints an answer that is 20.

printf("1. %d, 0x%x\n", 20, 20)

If you need help determining the answer, use C. For example,

    printf("1. %d, 0x%x\n", 0xaa | 0x55, 0xaa | 0x55);

But please study the answer until you understand the expression.

1. What is the value of 0xaa | 0x55?
2. What is the value of 0xaa & 0x55?
3. What is the value of 0xff000000 | 0x12345678?
4. What is the value of 0xff000000 & 0x12345678?
5. What is the value of 5 < 1?
6. What is the value of 5 << 1?
7. What is the value of 4 > 1?
8. What is the value of 4 >> 1?
9. What is the value of 5 / 2?
10. What is the value of 0xaa ^ 0x55?
11. What is the value of 0xa ^ 10?

 */

}
