#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "part.h"

part *new_part(char *name, int number, double cost) {
    part *p = malloc(sizeof(part));
    strcpy(p->name, name);
    p->number = number;
    p->cost = cost;
    return p;
}

void update_cost(part *p, double percent) {
    p->cost += p->cost * percent;
}

void print_part(part p) {
    printf("part - name: %10s, number: %7d, cost: $%7.2lf\n", p.name, p.number, p.cost);
}

void print_expensive(part * parts, int num_of_parts) {
    printf("Create this code\n");
}

part *find_longest(part *parts, int num_of_parts) {
    printf("Create this code\n");
    return NULL;
}

// return 0 on success and -1 on failure
int read_parts(char *filename, part parts[], int *num_of_parts) {
    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL)
        return -1;

    char buff[255];
    long l = 0;
    double d = 0;
    int i = 0;
    while (fscanf(fp, "%s %lu %lf", buff, &l, &d) != EOF) {
        strcpy(parts[i].name, buff);
        parts[i].number = l;
        parts[i].cost = d;
        i++;
    }
    fclose(fp);
    *num_of_parts = i;
    return 0;

}

// return 0 on success and -1 on failure
int read_parts_fancy(char *filename, part parts[], int *num_of_parts) {
    FILE *fp;
    if ((fp = fopen("in_data.txt", "r")) == NULL)
        return -1;

    part *p;
    int i = 0;
    for (i = 0, p = parts; fscanf(fp, "%s %d %lf", p->name, &p->number, &p->cost) != EOF; i++, p++);
    fclose(fp);
    *num_of_parts = i;
    return 0;
}


