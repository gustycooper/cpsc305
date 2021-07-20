#include <stdio.h>
#include <string.h>
int main() {
    char g[20] = "Gusty";
    if (strcmp(g, "Gusty") == 0)
      strcpy(g, "bicyclist");
    else if (strcmp(g, "Jerri Anne") == 0)
      strcpy(g, "chef");
    else if (strcmp(g, "Barrack") == 0)
      strcpy(g, "President");
    else
      strcpy(g, "nobody");
    printf("The occupation is %s\n", g);

    int num = -1;
    if (num > 0)   
       if (num <= 100) 
          printf("aaa\n");
    else
       printf("bbb\n");
    printf("done\n");

    if (num > 0) {
       if (num <= 100) 
          printf("aaa\n");
    } else
       printf("bbb\n");
    printf("done\n");

    if ((num > 0) && (num <= 100))
       printf("aaa\n");
    else
       printf("bbb\n");
    printf("done\n");
}
