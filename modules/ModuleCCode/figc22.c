#include <stdio.h>

#include <stdbool.h> // defines bool, true, false
int main() {
    bool tb = true, fb = false;
    tb=tb+tb+tb;
    int i=tb+tb+tb;
    int j=tb||fb;
    printf("tb: %d, fb: %d, i: %d, j: %d\n", tb, fb, i, j);
    _Bool TB = true, FB = false;
    TB=TB+TB+TB;
    i=TB+TB+TB;
    j=TB||FB;
    printf("TB: %d, FB: %d, i: %d, j: %d\n", TB, FB, i, j);
    i=true+true;j=false;
    printf("i: %d, j: %d\n", i, j);
}
