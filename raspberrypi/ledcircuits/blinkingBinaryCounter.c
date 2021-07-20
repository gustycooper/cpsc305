#include <stdio.h>
#include <wiringPi.h>

// Build: gcc -o blinkingBinaryCounter blinkingBinaryCounter.c -lwiringPi

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

/**
 * test if a bit is set
 * @param value - in which to test the bit
 * @param bit_pos - bit position to test
 * Example Call: bit_test(7, 2) returns 4
 */ 
int bit_test(int value, int bit_pos) {
    return CHECK_BIT(value, bit_pos);

// define wiringPi pins for LEDs
#define LED0 0
#define LED1 2
#define LED2 3

  int main(void) {
    // Setup WiringPi
    if (wiringPiSetup() == -1)
      return 1;

    // Set output pins
    pinMode(LED0, OUTPUT);
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);

    // Initialize pattern for blinking
    int led0[] = {0, 0, 0, 0, 1, 1, 1, 1};
    int led1[] = {0, 0, 1, 1, 0, 0, 1, 1};
    int led2[] = {0, 1, 0, 1, 0, 1, 0, 1};

    // Loop 100 times using led arrays
    for(int first_loop = 0; first_loop < 100; first_loop++) {
      for (int i=0; i<=7; i++) {
        printf("%d in decimal is %d%d%d in binary\n", i, led0[i], led1[i], led2[i]);
        digitalWrite(LED1, led0[i]);
        digitalWrite(LED2, led1[i]);
        digitalWrite(LED3, led2[i]);
        delay(1000);
      }
    }

    for (;;) {
        for(int i = 0; i < 8; i++) {
            int led0val = bit_test(i, 0) ? 1 : 0;
            int led1val = bit_test(i, 1) ? 1 : 0;
            int led2val = bit_test(i, 2) ? 1 : 0;
            digitalWrite(LED1, led0val);
            digitalWrite(LED2, led1val);
            digitalWrite(LED3, led2val);
            delay(1000);
        }
    }
    return 0;
  }
}
