#include "uart.h"

int putchar(char c)
{	
	uart_putchar(c);
	
	return 0;	
}

int puts(const char * s)
{	
	while (*s)
	{
		if (*s == '\n')
			uart_putchar('\r');
		uart_putchar(*s);
		
		s++;
	}
	
	return 0;
}

char *gets(char * s)
{
	char * buf = s;
	char c;
	
	while (1)
	{
		c = uart_getchar();
		
		// if c is user input "Enter", then end
		if (c == '\r')
			break;
			
		// if c is like a, b, c, d, then fill
		// if c is not '\b'
		if (c != '\b')
		{
			*buf++ = c;
			uart_putchar(c);
		}
		else
		{
			// if buf > s
			if (buf > s)
			{	
				buf--;
				//*buf = ' ';
				uart_putchar('\b');
				uart_putchar(' ');
				uart_putchar('\b');			
			}
		}
	}
	
	*buf = '\0';
	uart_putchar('\r');
	uart_putchar('\n');
	
	return buf;
}