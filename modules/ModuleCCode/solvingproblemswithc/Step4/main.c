#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "part.h"

int main(int argc, char **argv) {
    part pa[100];
    int num = 0;
    if (read_parts("part_data.txt", pa, &num) < 0) {
        perror("perror");
        exit(1);
    }

    for (int i = 0; i < num; i++)
        print_part(pa[i]);

    part *p = find_longest(pa, num);
    printf("part with longest name\n");
    if (p != NULL)
        print_part(*p);
    else
        printf("Multiple parts with same length.");

    part pa2[100];
    if (read_parts_fancy("part_data.txt", pa2, &num) < 0) {
        perror("perror");
        exit(1);
    }

    for (int i = 0; i < num; i++)
        print_part(pa2[i]);

    print_expensive(pa2, num);

    for (int i = 0; i < num; i++)
      update_cost(&pa2[i], .1);

    write_parts("part_data.txt", pa2, num);
    
}
