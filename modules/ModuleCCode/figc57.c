#include <stdio.h>
int main() {
  unsigned char a, b;
  printf("Enter two numbers [0 - 255]: ");
  scanf("%hhu %hhu", &a, &b);
  printf("%hhu & %hhu = %hhu\n",  a, b, (unsigned char)(a & b));
  printf("%hhu | %hhu = %hhu\n",  a, b, (unsigned char)(a | b));
  printf("%hhu ^ %hhu = %hhu\n",  a, b, (unsigned char)(a ^ b));
  printf("%hhu << %hhu = %hhu\n", a, b, (unsigned char)(a << b));
  printf("%hhu >> %hhu = %hhu\n", a, b, (unsigned char)(a >> b));
  printf("~%hhu = %hhu\n", a, (unsigned char)(~a));
  printf("~%hhu = %hhu\n", b, (unsigned char)(~b));
  return 0;
}
