
#include "uart.h"

void delay(void)
{
	int i;
	
	for (i = 0; i < 0x100000; i++)
		;
}

void putchar_hex(char c)
{
	char * hex = "0123456789ABCDEF";
	
	uart_putchar(hex[(c>>4) & 0x0F]);
	uart_putchar(hex[(c>>0) & 0x0F]);

	return;
}

int putchar(int c)
{
	// '\n' -> '\r' + '\n'
	if (c == '\n')
		uart_putchar('\r');
		
	uart_putchar(c);

	return 0;
}

