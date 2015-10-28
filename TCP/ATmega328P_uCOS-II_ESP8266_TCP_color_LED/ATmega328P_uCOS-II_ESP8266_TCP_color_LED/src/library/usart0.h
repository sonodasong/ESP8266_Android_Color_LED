#ifndef __USART0_H__
#define __USART0_H__

#include "define.h"

#define UART0_TX_TIMEOUT		0xFFFFFFFF

/* queue size is 2 ^ UART0_RX_Q_SIZE */
#define UART0_RX_Q_SIZE			2
/* buffer size is 2 ^ UART0_RX_SIZE */
#define UART0_RX_SIZE			5

void usart0Init(void);
INT8U usart0Print(char *buffer);
INT8U usart0LongInt(uint32 x);
INT8U usart0Read(char **str, INT32U timeout);

#endif
