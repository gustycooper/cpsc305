#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int global = 0;

int main(int argc, char *argv[]) {
    printf("Display addresses of program/process sections.\n");
    int *p = malloc(sizeof(int));
    assert(p != NULL);
    printf("code:   %15p\n", main);
    printf("static: %15p\n", &global);
    printf("heap:   %15p\n", p);
    printf("stack:  %15p\n", &p);
    printf("\nDisplay command invocation.\n%% ");
    for (int i = 0; i < argc; i++)
        printf("%s ", argv[i]);
    printf("\n");
    return 0;
}

