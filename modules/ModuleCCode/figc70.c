#include <stdio.h>
int main() {
    int a = 1;
    printf("switch without break\n");
    switch (a) {
       case 1:
          printf("case 1\n");
       case 2:
          printf("case 2\n");
       default:
          printf("default\n");
    }
    printf("switch with break\n");
    switch (a) {
       case 1:
          printf("case 1\n");
          break;
       case 2:
          printf("case 2\n");
          break;
       default:
          printf("default\n");
          break;
    }
}
