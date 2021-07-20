#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wiringPi.h>

/**
 * This circuit used the 5V pin to drive an LED
 * 
 *                                \=
 * pin 2 5V ---- 220Ohm ----- |LED > ----- C Transistor E ------ Ground
 *                                /               |
 *                                                | B
 *                                                |
 *                                               2.2KOhm
 *                                                |
 *                                                |
 *                                              pin 17 (0 for wiringPi)
 */

// Build: gcc -o transistorSwitch transistorSwitch.c -lwiringPi

#define NPN_GPIO 0    // wiringPi 0, GPIO17, Pin 11

int main() {
    if(wiringPiSetup() == -1){ //when initialize wiring failed,print messageto screen
        printf("setup wiringPi failed !");
        return 1; 
    }
    printf("Program is starting...\n");
    pinMode(NPN_GPIO, OUTPUT); 
    digitalWrite(NPN_GPIO, LOW); // turn off LED

    for (int i = 10; i > 0; i--) {
        int onOff = 0;
        printf("Enter 0 for off, 1 for on: ");
        scanf("%d", &onOff);
        if (onOff == 1) {
            printf("LED on\n");
            digitalWrite(NPN_GPIO, HIGH);  // turn on LED
        } else {
            print("LED off\n");
            digitalWrite(NPN_GPIO, LOW); // turn off LED
        }
            
    }
    return 0;
}
