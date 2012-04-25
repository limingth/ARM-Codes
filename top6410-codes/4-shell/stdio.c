#include "stdio.h"
#include "uart.h"

int puts(const char * s)
{
	char c;
	const char * buf = s;
	
	while ((c = *buf++) != '\0')
	{
		if (c == '\n')
			uart_putchar('\r');
			
		uart_putchar(c);	
	}
	
	return 	(buf - s);
}

char * gets(char * s)
{	
	char c;
	char * buf = s;
	
	while ((c = uart_getchar()) != '\r')
	{
		if (c != '\b')
		{
			*buf++ = c;
			uart_putchar(c);
		}
		else	// '\b'
		{
			if (buf > s)
			{	
				buf--;
				*buf = '\0';
				uart_putchar('\b');
				uart_putchar(' ');
				uart_putchar('\b');
			}
		}
	}
	
	*buf = '\0';
	uart_putchar('\r');
	uart_putchar('\n');
	
	return s;	
}
