#include <stdio.h>
#include <string.h>

int main() {
    //int[] ia = new int[10]; // Java allows brackets before the variable
    //int ia[] = new int[10]; // Java allows brackets after the variable 
    int a[20]; // In C brackets must be after the variable
    char s1[] = "Gusty";  // allocates 6 bytes for s1 with s1[5] == '\0'
    printf("%s\n", s1);   // prints Gusty to standard output
    s1[4] = 0;            // Replace 'y' with '\0'
    printf("%s\n", s1);   // prints Gust to standard output
    char c = s1[2];       // assign 's' to c
    char s2[5] = "Gusty"; // s2[4] == 'y' so s2 is not null terminated
    printf("%s\n", s2);   // prints Gusty and more until hitting a byte with 0
    char s3[10];          // allocates 10 bytes for s3
    //s3 = "Gusty";         // You cannot do this
    strcpy(s3, "Gusty");  // You can do this - see module C Functions for details
    printf("%s\n", s3);   // prints Gusty to standard output
}
