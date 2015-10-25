#include "usart0.h"

extern char hexTable[16];

#define usart0TxIntEnable()		UCSR0B |= ex(TXCIE0)
#define usart0TxIntDisable()	UCSR0B &= rex(TXCIE0)
#define usart0RxIntEnable()		UCSR0B |= ex(RXCIE0)
#define usart0RxIntDisable()	UCSR0B &= rex(RXCIE0)
#define usart0TxIntFlagClr()	UCSR0A |= ex(TXC0)

static char *usart0TxPtr;
static uint8 usart0TxCnt;
static OS_EVENT *usart0TxRdy;

static char usart0RxBuf[ex(UART0_RX_Q_SIZE)][ex(UART0_RX_SIZE)];
static uint8 usart0RxCnt;
static uint8 usart0RxQCnt;
static void *usart0RxQPtr[ex(UART0_RX_Q_SIZE)];
static OS_EVENT *usart0RxQ;

void usart0Init(void)
{
	UCSR0A = 0x02;
	UCSR0B = 0x18;
	UCSR0C = 0x06;
	UBRR0 = 207;
	
	usart0TxIntFlagClr();
	usart0TxIntEnable();
	usart0RxIntEnable();
	
	usart0TxRdy = OSSemCreate(0);
	usart0RxQ = OSQCreate(usart0RxQPtr, ex(UART0_RX_Q_SIZE));
	usart0RxCnt = 0;
	usart0RxQCnt = 0;
}

INT8U usart0Print(char *str)
{
	INT8U err;
	char temp;
	
	usart0TxCnt = 0;
	usart0TxPtr = str;
	temp = *str;
	if (temp != '\0') {
		UDR0 = temp;
		OSSemPend(usart0TxRdy, UART0_TX_TIMEOUT, &err);
	}
	return err;
}

INT8U usart0LongInt(uint32 x)
{
	char digit[11];
	uint8 i;
	
	digit[10] = '\0';
	i = 9;
	do {
		digit[i] = '0' + x%10;
		x /= 10;
		i--;
	} while (x != 0);
	i++;
	return usart0Print(&digit[i]);
}

INT8U usart0Read(char **str, INT32U timeout)
{
	INT8U err;
	
	*str = (char *)OSQPend(usart0RxQ, timeout, &err);
	return err;
}

static void usart0TxISRHandler(void)
{
	usart0TxCnt++;
	if (*(usart0TxPtr + usart0TxCnt) == 0) {
		OSIntEnter();
		OSSemPost(usart0TxRdy);
		OSIntExit();
	} else {
		UDR0 = usart0TxPtr[usart0TxCnt];
	}
}

static void usart0RxISRHandler(void)
{
	char usart1RxChar;
	
	usart1RxChar = UDR0;
	if (usart1RxChar == '\r') {
		usart0RxBuf[usart0RxQCnt][usart0RxCnt] = '\0';
	} else if (usart1RxChar == '\n') {
		if (usart0RxCnt != 0) {
			OSIntEnter();
			usart0RxBuf[usart0RxQCnt][usart0RxCnt] = '\0';
			usart0RxCnt = 0;
			OSQPost(usart0RxQ, usart0RxBuf[usart0RxQCnt]);
			usart0RxQCnt++;
			usart0RxQCnt &= ex(UART0_RX_Q_SIZE) - 1;
			OSIntExit();
		}
	} else {
		usart0RxBuf[usart0RxQCnt][usart0RxCnt] = usart1RxChar;
		usart0RxCnt++;
		usart0RxCnt &= ex(UART0_RX_SIZE) - 1;
	}
}

ISR(USART_TX_vect)
{
	usart0TxISRHandler();
}

ISR(USART_RX_vect)
{
	usart0RxISRHandler();
}
