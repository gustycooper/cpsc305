#include <stdio.h>

void f(int pbv, int *pbr) {
  pbv = 100;
  *pbr = 100;
}

void print_array(int *a, int size) {
    for (int i = 0; i < size; i++)
        printf("a[%d]: %d\n", i, a[i]);
}

int main() {
    int x = 10, y = 10;
    f(x, &y);
    printf("x = %d, y = %d\n", x, y);
    int arr[] = {1,2,3,4,5,6,7,8,9,10};
    print_array(arr, sizeof(arr)/sizeof(int));    
    int *p = arr;
    print_array(p, 10);  
}
