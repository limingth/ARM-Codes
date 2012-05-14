#include "uart.h"

int putchar(int c)
{
	if (c == '\n')
		uart_putchar('\r');
	
	if (c == '\b')
	{
		uart_putchar('\b');
		uart_putchar(' ');	
	}
			
	uart_putchar((char)c);
	
	return c;	
}

int getchar(void)
{
	int c;
	
	c = (int)uart_getchar();
	
	return c;
}

int puts(const char * s)
{
	while (*s)
		putchar(*s++);
		
	putchar('\n');

	return 0;
}

char * gets(char * s)
{
	char * p = s;
	
	while ((*p = getchar()) != '\r')
	{
		if (*p != '\b')
			putchar(*p++);
		else	
			if (p > s)
				putchar(*p--);	
	}
	
	*p = '\0';
	putchar('\n');
		
	return s;
}
