#include "esp8266.h"
#include "basic.h"
#include "usart0.h"

static void esp8266RxBufferFlash()
{
	INT8U err;
	char *str;
	
	do {
		err = usart0Read(&str, ESP8266_FLUSH_TIMEOUT);
	} while (err == OS_ERR_NONE);
}

boolean esp8266Cmd(char *cmd, char *response)
{
	INT8U err;
	char *str;
	
	esp8266RxBufferFlash();
	usart0Print(cmd);
	usart0Print("\r\n");
	while (1) {
		err = usart0Read(&str, ESP8266_SHORT_TIMEOUT);
		if (err == OS_ERR_TIMEOUT) {
			return FALSE;
		} else if (strCmp(str, response)) {
			return TRUE;
		}
	}
}

boolean esp8266ConnectAP(char *name, char *password)
{
	INT8U err;
	char *str;
	
	esp8266RxBufferFlash();
	usart0Print("AT+CWJAP=\"");
	usart0Print(name);
	usart0Print("\",\"");
	usart0Print(password);
	usart0Print("\"\r\n");
	while (1) {
		err = usart0Read(&str, ESP8266_LONG_TIMEOUT);
		if (err == OS_ERR_TIMEOUT) {
			return FALSE;
		} else if (strCmp(str, "OK")) {
			return TRUE;
		} else if (strCmp(str, "FAIL")) {
			return FALSE;
		}
	}
}

boolean esp8266GetIP(char *buf)
{
	INT8U err;
	char *str;
	boolean result;
	
	result = FALSE;
	esp8266RxBufferFlash();
	usart0Print("AT+CIFSR\r\n");
	while (1) {
		err = usart0Read(&str, ESP8266_SHORT_TIMEOUT);
		if (err == OS_ERR_TIMEOUT || strCmp(str, "OK")) {
			break;
		} else if (*str >= '0' && *str <= '9') {
			if (!strCmp(str, "192.168.4.1")) {
				strCpy(str, buf);
				result = TRUE;
			}
		}
	}
	return result;
}

char *esp8266Rx(void)
{
	char *str;
	while (1) {
		usart0Read(&str, ESP8266_INFINITE_TIMEOUT);
		if (*str == '+') {
			str += 8;
			while (*str != ':') {
				str++;
			}
			str++;
			break;
		}
	}
	return str;
}

void esp8266Tx(char *str)
{
	usart0Print("AT+CIPSEND=0,");
	usart0LongInt(strLen(str));
	usart0Print("\r\n");
	/* How to make sure get ">"? */
	usart0Print(str);
}
