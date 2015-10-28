#ifndef __BASIC_H__
#define __BASIC_H__

#include "define.h"

boolean strCmp(char *example, char *test);
void strCpy(char *example, char *buf);
uint8 strLen(char *str);
void enablePullup(void);
void disablePullup(void);
void ledInit(void);
void ledOn(void);
void ledOff(void);

#endif
