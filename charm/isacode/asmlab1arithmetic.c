#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    char line[100];
    float f;
    int i;
    while (1) {
        printf("Enter fp num: ");
        fgets(line, 100, stdin);
        if (line[0] == 'q')
            exit(1);
        sscanf(line, "%f", &f);
        memcpy(&i, &f, 4);
        printf("Num: %f, Hex: 0x%x\n", f, i);
    }
}
