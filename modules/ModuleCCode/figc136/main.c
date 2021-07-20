#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include "student.h"



int main() {
    int num = 0;
    student **students = read_students("students.txt", &num);
        for (int i = 0; i < num; i++) {
            print_student(students[i]);
            printf("student[%d]->name: %s\n", i, students[i]->name);
        }

    printf("highest GPA\n");
    print_student(highest_gpa(students, num));

    sort(students, num, NAME);
    printf("Sorted by name.\n");
    for (int i = 0; i < num; i++)
        print_student(students[i]);

    sort(students, num, AGE);
    printf("Sorted by age.\n");
    for (int i = 0; i < num; i++)
        print_student(students[i]);

    sort(students, num, GPA);
    printf("Sorted by gpa.\n");
    for (int i = 0; i < num; i++)
        print_student(students[i]);

    return 0;
}
