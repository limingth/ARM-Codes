
#include "uart.h"

void putchar_hex(char c)
{
	char * hex = "0123456789ABCDEF";
	
	uart_putchar(hex[(c>>4) & 0x0F]);
	uart_putchar(hex[(c>>0) & 0x0F]);

	return;
}

static void putchar(char c)
{
	// '\n' -> '\r' + '\n'
	if (c == '\n')
		uart_putchar('\r');
		
	uart_putchar(c);
}

int puts(const char * s)
{
	const char * p = s;
	
	while (*p)
		putchar(*p++);
		
	return (p - s);
}
