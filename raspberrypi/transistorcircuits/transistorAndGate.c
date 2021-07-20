#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>

/**
 * Program controls inputs to an AND gate created with transistors.
 * wiringPi pins 0 and 2 are the inputs to the AND gate
 * When both wiringPi pins 0 and 2 are high, the AND gate generates a True and lights and LED.
 * When either wiringPi pins 0 or 2 are low, the AND gate generate a False and LED remains unlit
 */

// Build: gcc -o transistorAndGate transistorAndGate.c -lwiringPi

#define AND_IN1_GPIO 0       // wiringPi 0, GPIO 17, Pin 11
#define AND_IN2_GPIO 2       // wiringPi 2, GPIO 27, Pin 13

int main() {
    if(wiringPiSetup() == -1) {
        printf("setup wiringPi failed !");
        return 1; 
    }
    printf("Program is starting...\n");
    pinMode(AND_IN1_GPIO, OUTPUT); 
    pinMode(AND_IN2_GPIO, OUTPUT); 
    digitalWrite(AND_IN1_GPIO, LOW); // set AND_IN1_GPIO to 0
    digitalWrite(AND_IN2_GPIO, LOW); // set AND_IN2_GPIO to 0
    
    for (int i = 10; i > 0; i--) {
        int and_in_1 = 0, and_in_2 = 0;
        printf("Enter Value for AND_IN1_GPIO: 0 for off, 1 for on: ");
        scanf("%d", &and_in_1);
        if (and_in_1 == 0)
            and_in_1 = LOW;
        else
            and_in_1 = HIGH;
        printf("Enter Value for AND_IN2_GPIO: 0 for off, 1 for on: ");
        scanf("%d", &and_in_2);
        if (and_in_2 == 0)
            and_in_2 = LOW;
        else
            and_in_2 = HIGH;
        digitalWrite(AND_IN1_GPIO, and_in_1); // set AND_IN1_GPIO to user's value
        digitalWrite(AND_IN2_GPIO, and_in_2); // set AND_IN2_GPIO to user's value
    }

    digitalWrite(AND_IN1_GPIO, LOW); // set AND_IN1_GPIO to 0
    digitalWrite(AND_IN2_GPIO, LOW); // set AND_IN2_GPIO to 0

    printf("HIGH: %d, LOW: %d\n", HIGH, LOW);

    return 0;
}
