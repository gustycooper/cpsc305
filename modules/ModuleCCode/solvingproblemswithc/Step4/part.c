#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "part.h"

part *new_part(char *name, int number, double cost, int count) {
    part *p = malloc(sizeof(part));
    strcpy(p->name, name);
    p->number = number;
    p->cost = cost;
    p->count = count;
    return p;
}

void update_cost(part *p, double percent) {
  p->cost += p->cost * percent;
}

void print_part(part p) {
  printf("Part - name: %10s, number: %7d, cost: $%7.2lf, count%7d\n", p.name, p.number, p.cost, p.count);
    
}

void print_expensive(part * parts, int num_of_parts) {
  int most = 0; // index of most expensive
  int count = 0; // count of most expensive
  for (int i = 1; i < num_of_parts; i++) {
    if (parts[i].cost > parts[most].cost) {
      most = i;
      count = 1;
    }
    else if (parts[i].cost == parts[most].cost)
      count++;
  }
  if (count == 1) {
    printf("Most expensive: ");
    print_part(parts[most]);
  }
  else
    printf("Multiple most expensive\n");
}

part *find_longest(part *parts, int num_of_parts) {
  part *longest = &parts[0];
  for (int i = 0; i < num_of_parts; i++) {
    if (strlen(longest->name) < strlen(parts[i].name))
      longest = &parts[i];
  }
          
  int count = 0;
  const part *end = &parts[num_of_parts];
  for (part *p = &parts[0]; p != end; p++) {
    if (strlen(longest->name) == strlen(p->name))
      count++;		
  }
          
  if (count > 1)
    return NULL;
  else
    return longest;
}

// return 0 on success and -1 on failure
int read_parts(char *filename, part parts[], int *num_of_parts) {
  FILE *fp;
  fp = fopen(filename, "r");
  if (fp == NULL)
    return -1;

  char buff[255];
  int n = 0, c = 0, i = 0;
  double d = 0;
  while (fscanf(fp, "%s %d %lf %d", buff, &n, &d, &c) != EOF) {
    strcpy(parts[i].name, buff);
    parts[i].number = n;
    parts[i].cost = d;
    parts[i].count = c;
    i++;
  }
  fclose(fp);
  *num_of_parts = i;
  return 0;
}

// return 0 on success and -1 on failure
int read_parts_fancy(char *filename, part parts[], int *num_of_parts) {
  FILE *fp;
  if ((fp = fopen(filename, "r")) == NULL)
    return -1;

  part *p;
  int i = 0;
  for (i = 0, p = parts; fscanf(fp, "%s %d %lf %d", p->name, &p->number, &p->cost, &p->count) != EOF; i++, p++);
  fclose(fp);
  *num_of_parts = i;
  return 0;
}

// return 0 on success and -1 on failure
int write_parts(char *filename, part parts[], int num_of_parts) {
  FILE *fp;
  if ((fp = fopen(filename, "w")) == NULL)
    return -1;

  part *p;
  int i = 0;
  for (i = 0, p = parts; i < num_of_parts; i++, p++)
    fprintf(fp, "%s %d %.2lf %d\n", p->name, p->number, p->cost, p->count);
  fclose(fp);
  return 0;
}


