// here we use uart_putchar() & uart_getchar() for raw data
#include "uart.h"

#define NAK 0x15
#define EOT 0x04
#define ACK 0x06

// delay time depend on external clock setting, see lib.c
extern void delay(void);

void xmodem_recv(char *addr)
{
	char c;
	int i;
	for (c = '9'; c > '0'; c--)
	{
		uart_putchar(c);
		delay();
	}
	uart_putchar(NAK);
	while (uart_getchar() != EOT)
	{
		uart_getchar(); 
		uart_getchar();
		for (i = 0; i < 128; i++)
			*addr++ = uart_getchar();
		uart_getchar();
		uart_putchar(ACK);
	}
	uart_putchar(ACK);
}
