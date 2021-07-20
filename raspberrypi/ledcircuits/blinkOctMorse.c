#include <wiringPi.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// Build: gcc -o blinkOctMorse blinkOctMorse.c -lwiringPi

#define  ledPin    0

#define DEBUG 0

char *morse[39] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....",
"..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", 
"...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", ".----", "..---", 
"...--", "....-", ".....", "-....", "--...", "---..", "----.", "-----", 
".-.-.-", "--..--", "..--.."};

char *alphabet[39] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", 
"K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", 
"Z", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "Stop", ",", "?"};

int main(void)
{
    if(wiringPiSetup() == -1){ //when initialize wiring failed,print messageto screen
        printf("setup wiringPi failed !");
        return 1; 
    }
    //when initialize wiring successfully,print message to screen    
    printf("wiringPi initialize successfully, GPIO %d(wiringPi pin)\n",ledPin);     
    
    pinMode(ledPin, OUTPUT);//Set the pin mode

    char letter[100];
    
    while(1){
        while (1) { // Turnon/Turnoff Loop
            printf("Enter: 1 turn on, 0 turn off, other goto Octal Blink: ");
            scanf("%s", letter);
            if (strcmp("1", letter) == 0)
                digitalWrite(ledPin, HIGH);
            else if (strcmp("0", letter) == 0)
                digitalWrite(ledPin, LOW);
            else
                break;
        }
        while (1) { // Octal Blink
            printf("Enter a number ($ to goto Morse Code): ");
            scanf("%s", letter);
            if (strcmp("$", letter) == 0)
                break;
            int num = atoi(letter) % 8;
            for (int i = 0; i < num; i++) {
                digitalWrite(ledPin, HIGH);
                delay(200);
                digitalWrite(ledPin, LOW);
                delay(200);
            }
        }
        while (1) { // Morse Code loop
            printf("Enter a letter ($ to goto turnonoff): ");
            scanf("%s", letter);
            if (strcmp("$", letter) == 0)
                break;
            int i;
            char in = letter[0];
            in = toupper(in);
            letter[0] = in;
            for (i = 0; i < 39; i++)
                if (strcmp(alphabet[i], letter) == 0)
                    break;
            printf("%d\n", i);
            char *code = morse[i];
            int delay_value;
            for (i = 0; i < strlen(code); i++) {
                if (code[i] == '.')
                    delay_value = 500;
                else
                    delay_value = 1500;
                digitalWrite(ledPin, HIGH);  //led on
                if (DEBUG) printf("led on...delay: %d\n", delay_value);
                delay(delay_value);
                digitalWrite(ledPin, LOW);  //led off
                if (DEBUG) printf("...led off\n");
                delay(delay_value);
            }
        }        
    }

    return 0;
}

