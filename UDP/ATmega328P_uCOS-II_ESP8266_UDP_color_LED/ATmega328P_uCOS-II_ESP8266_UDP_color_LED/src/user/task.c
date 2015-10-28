#include "task.h"

static boolean esp8266Initialized = FALSE;

static uint8 hexToInt(char *hex)
{
	return ((hex[0] - '0') << 4) | ((hex[1] - '0') & 0x0F);
}

void blink(void *pdata)
{
	(void)pdata;
	while (1) {
		if (esp8266Initialized) {
			ledOn();
			OSTimeDly(1);
			ledOff();
			OSTimeDly(29);
			ledOn();
			OSTimeDly(1);
			ledOff();
			OSTimeDly(219);
		} else {
			ledOn();
			OSTimeDly(30);
			ledOff();
			OSTimeDly(30);
		}
	}
}

void colorLEDDemo(void *pdata)
{
	uint16 hue;
	
	(void)pdata;
	hue = 0;
	while (1) {
		if (hue == 240) {
			hue = 0;
		} else {
			hue++;
		}
		ledRedSet(hueToRed(hue));
		ledGreenSet(hueToGreen(hue));
		ledBlueSet(hueToBlue(hue));
		OSTimeDly(6);
	}
}

void esp8266ColorLED(void *pdata)
{
	char *str;
	char buf[32];
	uint8 red;
	uint8 green;
	uint8 blue;
	uint8 hue;
	
	(void)pdata;
	red = 0;
	green = 0;
	blue = 0;
	hue = 0;
	while (!esp8266Initialized) {
		while (1) {
			if (!esp8266Cmd("AT+RST", "ready")) {
				break;
			}
			if (!esp8266Cmd("ATE0", "OK")) {
				break;
			}
			if (!esp8266Cmd("AT+CIPMUX=1", "OK")) {
				break;
			}
			if (!esp8266Cmd("AT+CIPSERVER=1, 5678", "OK")) {
				break;
			}
			esp8266Initialized = TRUE;
			break;
		}
	}
	while (1) {
		str = esp8266Rx();
		if (str[0] == 'R') {
			red = hexToInt(str + 1);
			ledRedSet(red);
			ledGreenSet(green);
			ledBlueSet(blue);
		} else if (str[0] == 'G') {
			green = hexToInt(str + 1);
			ledRedSet(red);
			ledGreenSet(green);
			ledBlueSet(blue);
		} else if (str[0] == 'B') {
			blue = hexToInt(str + 1);
			ledRedSet(red);
			ledGreenSet(green);
			ledBlueSet(blue);
		} else if (str[0] == 'H') {
			hue = hexToInt(str + 1);
			ledRedSet(hueToRed(hue));
			ledGreenSet(hueToGreen(hue));
			ledBlueSet(hueToBlue(hue));
		} else if (str[0] == 'N') {
			strCpy(str + 1, buf);
		} else if (str[0] == 'P') {
			if (esp8266ConnectAP(buf, str + 1)) {
				esp8266Tx("OK");
			} else {
				esp8266Tx("FAIL");
			}
		} else if (str[0] == 'I') {
			if (esp8266GetIP(buf)) {
				esp8266Tx(buf);
			} else {
				esp8266Tx("FAIL");
			}
		}
	}
}
