#include <wiringPi.h>

#define PININ1 8
#define PINOUT1 9
#define PININ2 7
#define PINOUT2 15
#define PININ3 16
#define PINOUT3 1

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