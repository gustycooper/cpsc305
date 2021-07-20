
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

/**
 * find the first bit set in value, beginning with bit position 0
 * @param value the value to search for a bit set
 * @return the first bit position set or -1 if value is 0
 */
int bit_find(int value) {    
    for (int i = 0; i < 32; i++)
        if (CHECK_BIT(value, i))
            return i;
    return -1;
}

int bit_test(int value, int bit_pos) {
    return CHECK_BIT(value, bit_pos);
}

void bit_set(int *value, int bit_pos) {
    *value |= 1 << bit_pos;
} 

void bit_clear(int *value, int bit_pos) {
    *value &= ~(1 << bit_pos);
}
