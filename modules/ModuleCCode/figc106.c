#include <stdio.h>
int main() {
    int a[] = {1,3,5,7,9};
    FILE *fout = fopen("figc106out_bin", "w");
    int count = fwrite(a, sizeof(int), 5, fout);
    fclose(fout);
    printf("count: %d\n", count);
    return 0;
}
