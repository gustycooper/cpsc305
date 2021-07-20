#include <stdio.h>
// invoke as ./a.out one two three
int main(int argc, char **argv) {
    for (int i = 0; i < argc; i++)
        printf("Argument %d is '%s'\n", i, argv[i]); 
    return 0;
}
