#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>

// Build: gcc -o interrupt interrupt.c -lwiringPi

#define NPN_GPIO 0        // wiringPi 0, GPIO17, Pin 11
#define BUTTON_GPIO 2     // wiringPi 2, GPIO 27, Pin 13
#define DEBOUNCE_TIME 200 // debounce time in ms

int count_button_presses;
int count_debouncing;
long current_isr_time;
long last_isr_time;
int led_state = LOW;      // off

void switch_onoff_LED(void) {
    current_isr_time = millis();
    count_debouncing++;
    if (current_isr_time - last_isr_time > DEBOUNCE_TIME) {
        count_button_presses++;
        led_state = !led_state;
        digitalWrite(NPN_GPIO, led_state);
        printf("LED state: %d\n", led_state);
        printf("Button pressed %d times.\n", count_button_presses);
        printf("Debounce %d times.\n", count_debouncing);
        count_debouncing = 0;
    }
    last_isr_time = current_isr_time;
}

int main() {
    if(wiringPiSetup() == -1){ //when initialize wiring failed,print messageto screen
        printf("setup wiringPi failed !");
        return 1; 
    }
    printf("Program is starting...\n");
    pinMode(NPN_GPIO, OUTPUT); 
    pinMode(BUTTON_GPIO, INPUT);
    digitalWrite(NPN_GPIO, LOW); // turn off LED
    
    // Establish lightLED function as Interrupt Service Routine (ISR)
    wiringPiISR(BUTTON_GPIO, INT_EDGE_RISING, &switch_onoff_LED);

    printf("Press button to turn on/off LED...\n");

    for (int i = 60; i > 0; i--) {
        //printf("You have %d seconds remaining...\n", i);
        //printf("Press button to turn on/off LED...\n");
        sleep(1);
    }

    digitalWrite(NPN_GPIO, LOW); // turn off LED before exiting

    return 0;
}
