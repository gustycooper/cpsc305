#include <stdio.h>
#define ARRAY_SIZE 10
#define PI 3.1415
#define CIRCLE_AREA(r) (PI*r*r)
#define BAD_SQUARE(x) (x * x)
#define SQUARE(x) ((x) * (x)) 

int a[ARRAY_SIZE];

void read_array(int *a, int s) {
    printf("This can read values into the int a[] of size s\n");
}

int main() {
    for (int i = 0; i < ARRAY_SIZE; i++)
        printf("%d\n", a[i]);
    double area = CIRCLE_AREA(4.9);
    int radius = (int)10.3;
    area = CIRCLE_AREA(radius);
    int x = BAD_SQUARE(3 + 4);
    x = SQUARE(3 + 4);
    x = SQUARE(x++);
    int a[ARRAY_SIZE];
    read_array(a, ARRAY_SIZE);
}
