#include "led.h"
#include "pwm.h"

void ledRedSet(uint8 red)
{
	pwm2B(red);
}

void ledGreenSet(uint8 green)
{
	pwm0B(green);
}

void ledBlueSet(uint8 blue)
{
	pwm0A(blue);
}
