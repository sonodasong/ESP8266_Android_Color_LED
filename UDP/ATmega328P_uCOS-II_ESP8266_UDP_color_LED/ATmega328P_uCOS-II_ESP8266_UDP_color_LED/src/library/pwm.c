#include "pwm.h"

static void pwm0Init(void);
static void pwm2Init(void);

void pwmInit(void)
{
	pwm0Init();
	pwm2Init();
}

void pwm0A(uint8 dutyCycle)
{
	OCR0A = ~dutyCycle;
}

void pwm0B(uint8 dutyCycle)
{
	OCR0B = ~dutyCycle;
}

void pwm2A(uint8 dutyCycle)
{
	OCR2A = ~dutyCycle;
}

void pwm2B(uint8 dutyCycle)
{
	OCR2B = ~dutyCycle;
}

static void pwm0Init(void)
{
	TCNT0 = 0;
	OCR0A = 255;
	OCR0B = 255;
	/* important to get 0 duty cycle */
	TCCR0A = 0xF3;
	TCCR0B = 0x03;
	TIMSK0 = 0x00;
	DDRD |= ex(5) | ex(6);
}

static void pwm2Init(void)
{
	TCNT2 = 0;
	OCR2A = 255;
	OCR2B = 255;
	/* important to get 0 duty cycle */
	TCCR2A = 0xF3;
	TCCR2B = 0x03;
	TIMSK2 = 0x00;
	DDRB |= ex(3);
	DDRD |= ex(3);
}
