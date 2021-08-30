#include "struct_test.h"

int printf(char *format, ...);

struct person p = {"Gusty", 22};
void struct_test() {
    printf("struct test");
    printf("Name: Gusty age: %d", p.age);
}
