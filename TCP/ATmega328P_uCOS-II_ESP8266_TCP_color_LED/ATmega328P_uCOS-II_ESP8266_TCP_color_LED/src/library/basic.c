#include "basic.h"

boolean strCmp(char *example, char *test)
{
	char e;
	uint8 i;
	
	i = 0;
	do {
		e = example[i];
		if (e != test[i]) {
			return FALSE;
		}
		i++;
	} while (e != '\0');
	return TRUE;
}

void strCpy(char *example, char *buf)
{
	char e;
	uint8 i;
	
	i = 0;
	e = *example;
	*buf = e;
	while (e != '\0') {
		i++;
		e = example[i];
		buf[i] = e;
	}
}

uint8 strLen(char *str)
{
	uint8 i;
	
	i = 0;
	while(str[i] != '\0') {
		i++;
	}
	return i;
}

void enablePullup(void)
{
	MCUCR &= rex(PUD);
}

void disablePullup(void)
{
	MCUCR |= ex(PUD);
}

void ledInit(void)
{
	DDRB |= ex(5);
}

void ledOn(void)
{
	PORTB |= ex(5);
}

void ledOff(void)
{
	PORTB &= rex(5);
}
