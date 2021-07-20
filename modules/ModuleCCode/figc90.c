#include <stdio.h>
#include <stdarg.h>

void little_printf(char *fmt, ...) {
  va_list ap;
  char *p, *sval;
  int ival;
  double dval;
  va_start(ap, fmt);
  for (p = fmt; *p; p++) {
    if (*p != '%') {
      putchar(*p);
      continue;
    }
    switch (*++p) {
      case 'd':
        ival = va_arg(ap, int);
        printf("%d", ival);
        break;
     case 'f':
        dval = va_arg(ap, double);
        printf("%f", dval);
        break;
     case 's':
        for (sval = va_arg(ap, char *); *sval; sval++)
          putchar(*sval);
        break;
     default:
         putchar(*p);
         break;
    }
  }
  va_end(ap);
}

int min(int argc, ...) { 
  int min, a; 
  va_list ap; 
  va_start(ap, argc); 
  min = va_arg(ap, int); 
  for (int i = 2; i <= argc; i++) 
    if ((a = va_arg(ap, int)) < min) 
      min = a; 
  va_end(ap); 
  return min; 
} 

int main() {
  little_printf("little_printf: %d, %f, %s\n", 5, 3.14159, "Hello");
  little_printf("Minimum value is %d\n", min(5, 12, 67, 6, 7, 100)); 
  return 0;
}
