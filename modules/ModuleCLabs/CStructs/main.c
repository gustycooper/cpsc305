#include <stdio.h>
#include <string.h>
#include "cup.h"

static char *contents_to_str[] = {"TEA", "COFFEE", "WATER", "MILK", "MIXED"};
contents str_to_contents(char *s) {
    for (int i = 0; i < 5; i++)
        if (strcmp(s, contents_to_str[i]) == 0)
            return i;
    return 4;
}

static char *material_to_str[] = {"PAPER", "GLASS", "PLASTIC"};
enum material str_to_material(char *s) {
    for (int i = 0; i < 3; i++)
        if (strcmp(s, material_to_str[i]) == 0)
            return i;
    return 2;
}

int main() {
    cup c = {12, 10, TEA, PAPER, "red"};
    printf("print_cup(c) - intial values: ");
    print_cup(&c);
    printf("status from overflowing cup: %d\n", fill(&c, MILK, 10));
    printf("print_cup(c) - after filling: ");
    print_cup(&c);
    cup *p = new_cup(30, 24, WATER, GLASS, "pink");
    printf("print_cup(*p): ");
    print_cup(p);
    cup ca[] = {c, {16, 9, COFFEE, GLASS, "blue"} , *p, {21, 20, WATER, PLASTIC, "green"} };
    cup *ca_p[] = {&ca[0], &ca[1], &ca[2], &ca[3]};
    printf("print_cups(ca, 4, WATER):\n");
    print_cups(ca_p, 4);
    printf("print_cup(fine_fullest(ca_p, 4)): ");
    print_cup(find_fullest(ca_p, 4));

// red 12 12 TEA PAPER
    cup cups[100], *cups_p[100], *cups_m[100];
    int count = 0;
    char color[16], contents[16], material[16];
    int cur, max;
    while (scanf("%s %d %d %s %s", cups[count].color, &cups[count].max_oz, &cups[count].cur_oz, contents, material) != EOF) {
        cups[count].contents = str_to_contents(contents);
        cups[count].material = str_to_material(material);
        cup *cp = &cups[count];
        cups_p[count] = cp;
        cups_m[count] = new_cup(cp->max_oz, cp->cur_oz, cp->contents, cp->material, cp->color);
        count++;
    }
    // cups is an array of cup - each element is a cup
    printf("\ncups[] values\n");
    for (int i = 0; i < count; i++) {
        printf("cup[%d]: ", i);
        print_cup(&cups[i]);
    }

    // cups_p is an array of pointers to a cup.
    // Initially cups_p[i] = & cups[i]
    // When sorted, cups_p[i] points to some other element in cups.
    // If you modify an element in cups[], the cups_p[] pointer points to a modified element
    printf("\ncups_p[] values, each element points to a cups[]\n");
    print_cups(cups_p, count);
    printf("print_cup(find_fullest(cups_p, count)): ");
    print_cup(find_fullest(cups_p, count));


    // cups_m is an array of pointers to a cup.
    // Each element of cups_m[] is created via new_cup
    // The memory for each pointer is on the heap
    printf("\ncups_m[] values, each element points to a cup on the heap\n");
    print_cups(cups_m, count);

    printf("\nsorted version of cups_p\n");
    sort_cups(cups_p, count);
    print_cups(cups_p, count);

    printf("\nsorted version of cups_m\n");
    sort_cups(cups_m, count);
    print_cups(cups_m, count);

    // modify cups[4], which has purple 20 20 WATER PAPER
    cups[4].cur_oz = 0;
    printf("\nreprint cups_p after changing cups[4].cur_oz = 0\n");
    print_cups(cups_p, count);
    printf("\nreprint cups_m after changing cups[4].cur_oz = 0\n");
    print_cups(cups_m, count);


}
