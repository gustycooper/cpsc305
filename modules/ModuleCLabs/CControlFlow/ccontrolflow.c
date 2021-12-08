#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(int argc, char **argv) {
    double a[1024], *p = a;
    int aelems = 0;
    while (scanf("%lf", p++) != EOF) // fill a with values from stdin
        aelems++; // count elemenst in a
    // At this point a is filled with aelems
    for (int i = 0; i < aelems; i++)
        printf("a[%d]: %8.2lf\n", i, a[i]);

/* 1.
Place your code described in 1 after this comment
 */

/*
Data structures used by 2, 3, and 4.
 */
    char *cp = malloc(sizeof("C programming is fun!"));
    strcpy(cp, "C programming is fun!");
    char ca[] = "C programming is fun!";

/* 2a.
 What does While Loop 1 compute?
 Explain While Loop 1.
 */
    int c1 = 0;
    while (ca[c1] != 0)
       c1++;
    printf("while loop 1: %d\n", c1);
/* 2b.
 What does While Loop 2 compute?
 Explain While Loop 2.
 */
    int c2 = 0;
    char *cap = ca;
    while (*cap++ !=0)
       c2++;
    printf("while loop 2: %d\n", c2);
/* 3a.
 What does For Loop 1 compute?
 Explain For Loop 1.
 */
    for (c1 = 0; ca[c1] != 0; c1++);
    printf("for loop 1: %d\n", c1);
    
/* 3b.
 What does For Loop 2 compute?
 Explain For Loop 2.
 */
    c2 = 0;
    for (cap = ca; *cap != 0; cap++)
        c2++;
    printf("for loop 2: %d\n", c2);

/* 4. Analyze and explain the following code
   4a. What does the while loop and the immediately following if statement compute?
   4b. What causes the while loop to terminate?
   4c. Explain how the while loop and the immediately following if statement work.
   4c. What is goto_loop:?
   4d. Explain how the code after goto_loop: is executed 3 times.
 */
    char *g1 = "Gusty", g2[] = "Gustz", g = 0;
goto_loop:
    g = 0;
    while (g1[g]) {
        if (g1[g] != g2[g])
            break;
        g++;
    }
    if (g1[g] - g2[g] < 0)
        printf("%s, %s: Negative\n", g1, g2);
    else if (g1[g] - g2[g] == 0)
        printf("%s, %s: Zero\n", g1, g2);
    else
        printf("%s, %s: Postive\n", g1, g2);
    if (g2[4] == 'z') {
        g2[4] = 'y';
        goto goto_loop;
    }
    else if (g2[4] == 'y') {
        g2[4] = 'a';
        goto goto_loop;
    }    
}
