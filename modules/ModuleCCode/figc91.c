#include <stdio.h> 

int fact(int x) {
    return x == 0 ? 1 : x * fact(x - 1); 
}

int main() {
    printf("%d! = %d\n", 10, fact(10)); return 0;
}
