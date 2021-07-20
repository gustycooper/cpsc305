#include <stdio.h>
#include <string.h>
int main() {
    int c = 1;
    printf("while loop\n");
    while (c <= 5) {
      printf("%d\n", c);
      c++;
    }
    printf("for loop\n");
    for (int c = 1; c<=5; c++)
      printf("%d\n", c);

    printf("nested loops\n");
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            printf("%d + %d = %d\n", i, j, i+j);


    printf("loop with break\n");
    char s[] = "Coffee cup.";
    int i = 0;
    while (i < strlen(s)) {
       if (s[i] == ' ')
          break;
       i++;
    }
    if (i == strlen(s))
       printf("%s does not contain a space\n", s);
    else
       printf("First space in %s is at position %d\n", s, i);

    printf("loop with continue\n");
    for (int j = 1; j <= 10; j++) {
       if (j == 5)
          continue;
       printf("%d ", j);
    }
    printf("\n");
}
