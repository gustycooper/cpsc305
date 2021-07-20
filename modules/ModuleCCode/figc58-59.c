#include <stdio.h>

void set_bit(unsigned int* value, int bit_number) {
    int mask = (1 << bit_number);
    *value = *value | mask;
}
void clear_bit(unsigned int* value, int bit_number) {
    int mask = ~(1 << bit_number);
    *value = *value & mask;
}
int test_bit(unsigned int value, int bit_number) {
    int mask = 1 << bit_number;
    if (value & mask)
        return 1;
    else
        return 0;
}

int test_bit_c(unsigned int value, int bit_number) {
  return (value & 1 << bit_number) ? 1 : 0;
}

int main() {
    unsigned int val = 0, *p = &val;
    set_bit(&val, 4);
    printf("val: %d\n", val);
    set_bit(p, 3);
    printf("val: %d\n", val);
    clear_bit(p, 3);
    printf("val: %d\n", val);
    printf("test_bit(val, 4): %d\n", test_bit(val, 4));
    printf("test_bit_c(val, 4): %d\n", test_bit_c(val, 4));
    printf("test_bit(val, 3): %d\n", test_bit(val, 3));
    printf("test_bit_c(val, 3): %d\n", test_bit_c(val, 3));
}
