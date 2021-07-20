#include <stdio.h>
int main() {
  FILE *fin  = fopen("figc102in_data.txt",  "r");
  FILE *fout = fopen("figc102out_data.txt", "w+");
  int number, count = 0;
  char name[50];
  double cost;
  fprintf(fout, "Parts in Database\n");
  while (fscanf(fin, "%s %d %lf", name, &number, &cost) != EOF) {
    // do something with name, number and cost
    fprintf(stdout, "Part - Name: %s, Number: %d Cost: $%.2lf\n", name, number, cost);
    fprintf(fout, "Part - Name: %s, Number: %d Cost: $%.2lf\n", name, number, cost);
    count++; // count lines read
  }
  fclose(fin);
  fclose(fout);

  int a[] = {1,3,5,7,9};
  fout = fopen("out_bin", "w");
  count = fwrite(a, sizeof(int), 5, fout);
  fclose(fout);
  return 0;
}
