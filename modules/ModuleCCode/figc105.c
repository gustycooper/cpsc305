#include <stdio.h>
int main() {
    char line[100];
    printf("Enter a line: ");
    gets(line);
    printf("%s\n", line);
    printf("Enter a line: ");
    fgets(line, 99, stdin);
    printf("%s\n", line);
    return 0;
}
