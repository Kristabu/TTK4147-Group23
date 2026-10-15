#include <wiringPi.h>

#define PININ1 8
#define PINOUT1 9
#define PININ2 7
#define PINOUT2 15
#define PININ3 16
#define PINOUT3 1

#define _GNU_SOURCE

void io_init(void)
{
    wiringPiSetup();

    pinMode(PININ1, INPUT);
    pinMode(PINOUT1, OUTPUT);
    pinMode(PININ2, INPUT);
    pinMode(PINOUT2, OUTPUT);
    pinMode(PININ3, INPUT);
    pinMode(PINOUT3, OUTPUT);

    digitalWrite(PINOUT1, HIGH);
    digitalWrite(PINOUT2, HIGH);
    digitalWrite(PINOUT3, HIGH);
}

int main(void)
{
    // uses BCM numbering of the GPIOs and directly accesses the GPIO registers.
    wiringPiSetupGpio();

    // pin mode ..(INPUT, OUTPUT, PWM_OUTPUT, GPIO_CLOCK)
    // set pin 17 to input
    pinMode(17, INPUT);

    // pull up/down mode (PUD_OFF, PUD_UP, PUD_DOWN) => down
    pullUpDnControl(17, PUD_DOWN);

    // get state of pin 17
    int value = digitalRead(17);

    if (HIGH == value)
    {
        // your code
    }
    /*
    compile command
    clang main.c -o main -std=gnu11 -lwiringPi -lwiringPiDev

    picocom -b 9600 /dev/ttyS0 --flow none --stopbits 1 --imap lfcrlf
    You may have to use sudo and/or change S0 to S1 or similar to open the correct serial port.
    */
}