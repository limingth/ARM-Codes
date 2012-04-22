#include "uart.h"

void putchar(char c)
{
	if (c == '\n')
		uart_putchar('\r');
		
	if (c == '\b')
	{
		uart_putchar('\b');
		uart_putchar(' ');
	}	
		
	uart_putchar(c);
}

char getchar()
{
	return uart_getchar();
}

void puts(char *s)
{
	while(*s)
		putchar(*s++);
}

char * gets(char * s)
{
	char c;
	char * buf = s;
	
	while((c = getchar()) != '\r')
	{
		if (c != '\b')	// normal case
		{
			putchar(c);
			*buf++ = c;
		}
		else	// exception case
		{
			if (buf > s)	// more condition
			{
				putchar(c);
				buf--;
			}
		}
	}
	
	*buf = '\0';
	putchar('\n');
	
	return s;
}