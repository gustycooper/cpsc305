#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"
#include "split.h"

student *new_student(char *n,int a,double g) {
    student *s = malloc(sizeof(student));
    strcpy(s->name, n);
    s->age = a;
    s->gpa = g;
    return s;
}

void print_student(student *s) {
  printf("name: %s, age: %d, gpa: %.2lf\n", s->name, s->age, s->gpa);
}

student **read_students(char *filename, int *num) {
  FILE *fp;
  fp = fopen(filename, "r");
  if (fp == NULL)
    return NULL;
  char line[100] = { 0 };
  int num_students = 0; 
  while (fgets(line, 100, fp) != NULL) // count students in file
    num_students++;
  fclose(fp);
  student **students = malloc(num_students * sizeof(student *));
  fp = fopen(filename, "r");
  num_students = 0;
  while (fgets(line, 100, fp) != NULL) {
    int num_strs = 0;
    char **splits = split(line, " ", &num_strs);
    students[num_students++] = new_student(splits[0], atoi(splits[1]), atof(splits[2]));
    for (int i = 0; i < num_strs; i++)
      free(splits[i]);
    free(splits);
  }
  *num = num_students;
  fclose(fp);
  return students;
}

student *highest_gpa(student **students, int num) {
  student *high_gpa = students[0];
  for (int i = 0; i < num; i++)
    if (students[i]->gpa > high_gpa->gpa)
      high_gpa = students[i];
  return high_gpa;
}

void sort(student **students, int num, enum sort_type st) {
    for (int i = 0; i < num; i++) {
        for (int j = i; j < num; j++) {
            switch (st) {
              case GPA:
                if (students[i]->gpa > students[j]->gpa) {
                    student *p = students[i];
                    students[i] = students[j];
                    students[j] = p;
                }
                break;
              case AGE:
                if (students[i]->age > students[j]->age) {
                    student *p = students[i];
                    students[i] = students[j];
                    students[j] = p;
                }
                break;
              case NAME:
                if (strcmp(students[i]->name, students[j]->name) > 0) {
                    student *p = students[i];
                    students[i] = students[j];
                    students[j] = p;
                }
                break;
            }
        }
    }
}
