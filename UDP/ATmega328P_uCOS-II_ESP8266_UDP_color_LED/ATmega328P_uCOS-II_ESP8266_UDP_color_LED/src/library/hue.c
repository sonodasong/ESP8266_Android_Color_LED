#include "hue.h"

uint16 hueToRed(uint16 hue)
{
	uint16 red;
	
	if (hue < 40) {
		red = 240;
	} else if (hue < 80) {
		red = 480 - hue * 6;
	} else if (hue < 160) {
		red = 0;
	} else if (hue < 200) {
		red = hue * 6 - 960;
	} else {
		red = 240;
	}
	return red;
}

uint16 hueToGreen(uint16 hue)
{
	uint16 green;
	
	if (hue < 40) {
		green = hue * 6;
	} else if (hue < 120) {
		green = 240;
	} else if (hue < 160) {
		green = 960 - hue * 6;
	} else {
		green = 0;
	}
	return green;
}

uint16 hueToBlue(uint16 hue)
{
	uint16 blue;
	
	if (hue < 80) {
		blue = 0;
	} else if (hue < 120) {
		blue = hue * 6 - 480;
	} else if (hue < 200) {
		blue = 240;
	} else {
		blue = 1440 - hue * 6;
	}
	return blue;
}
