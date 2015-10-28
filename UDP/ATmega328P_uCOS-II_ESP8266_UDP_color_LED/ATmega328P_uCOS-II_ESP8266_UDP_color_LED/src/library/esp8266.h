#ifndef __ESP8266_H__
#define __ESP8266_H__

#include "define.h"

#define ESP8266_INFINITE_TIMEOUT		0xFFFFFFFF
#define ESP8266_LONG_TIMEOUT			0x3FFF
#define ESP8266_SHORT_TIMEOUT			0x03FF
#define ESP8266_FLUSH_TIMEOUT			0x0F

boolean esp8266Cmd(char *cmd, char *response);
boolean esp8266ConnectAP(char *name, char *password);
boolean esp8266GetIP(char *buf);
char *esp8266Rx(void);
void esp8266Tx(char *str);

#endif
