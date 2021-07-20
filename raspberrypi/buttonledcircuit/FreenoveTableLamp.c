/**********************************************************************
* Filename    : FreenoveTableLamp.c
* Description : a DIY MINI table lamp
* Author      : freenove
* modification: Gusty edited the code
Build: gcc -o FreenoveTableLamp FreenoveTableLamp.c -lwiringPi
**********************************************************************/
#include <wiringPi.h>
#include <stdio.h>

#define ledPin    0      //define the ledPin
#define buttonPin 1      //define the buttonPin

int ledState=LOW;        //store the State of led
int buttonState=HIGH;    //store the State of button
int lastbuttonState=HIGH;//store the lastState of button
long lastChangeTime;     //store the change time of button state
long captureTime=50;     //set the button state stable time
int reading;

int main(void) {
    if(wiringPiSetup() == -1) {
        printf("setup wiringPi failed !");
        return 1; 
    }
    printf("Program is starting...\n");
    pinMode(ledPin, OUTPUT); 
    pinMode(buttonPin, INPUT);
    pullUpDnControl(buttonPin, PUD_UP);   // pull up to high level
    while(1){
        reading = digitalRead(buttonPin); // read the current state of button
        if( reading != lastbuttonState){  // if the button state has changed, record time
            lastChangeTime = millis();
        }
        //if changing-state lasts beyond time, button state is stable, debounce over
        if(millis() - lastChangeTime > captureTime) {
            if(reading != buttonState) {  // if button state changed, update the data.
                buttonState = reading;
                if(buttonState == LOW) {  // if state is low, action is pressing
                    printf("Button is pressed!\n");
                    ledState = !ledState; // Update LED state .
                    if(ledState) {
                        printf("turn on LED ...\n");
                    }
                    else {
                        printf("turn off LED ...\n");
                    }
                }
                else { //if state high, action is releasing
                    printf("Button is released!\n");
                }
            }
        }
        digitalWrite(ledPin,ledState);
        lastbuttonState = reading;
    }
    return 0;
}
