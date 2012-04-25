
#include "uart.h"

void putchar_hex(char c)
{
	char * hex = "0123456789ABCDEF";
	
	uart_putchar(hex[(c>>4) & 0x0F]);
	uart_putchar(hex[(c>>0) & 0x0F]);

	return;
}

void putchar(char c)
{
	// '\n' -> '\r' + '\n'
	if (c == '\n')
		uart_putchar('\r');
		
	uart_putchar(c);
}

void putint_hex(int n)
{
	putchar_hex((n>>24) & 0xFF);
	putchar_hex((n>>16) & 0xFF);
	putchar_hex((n>>8) & 0xFF);
	putchar_hex((n>>0) & 0xFF);
	uart_putchar(' ');
	
	return;
}

int puts(const char * s)
{
	const char * p = s;
	
	while (*p)
		putchar(*p++);
		
	return (p - s);
}

int strlen(const char * s)
{
	const char * p = s;
	
	while (*p)
		p++;
		
	return (p - s);
}

char * reverse(char * s)
{
	static int counter = 1;
	static char * p;	
	char save;
	
	if (counter == 1)
		p = s;
	
	counter++;	
	
	if (*s == '\0')
	{
		counter = 1;
		return s;
	}
	
	save = s[0];	
	reverse(s+1);	
	*p++ = save;	
	
	return s;
}

char * reverse_r(char * s)
{
	int len = strlen(s);
	char c1 = s[0];
	char c2 = s[len-1];
	
	if (*s == '\0')
		return s;
		
	s[len-1] = '\0';
	
	reverse(s+1);
	
	s[0] = c2;
	s[len-1] = c1;
	
	return s;
}

char * reverse0(char * s)
{
	int len = strlen(s);
	int i;
	
	for (i = 0; i < len / 2; i++)
	{
		char tmp;
		
		tmp = s[i];
		s[i] = s[len-i-1];
		s[len-i-1] = tmp;
	}
	
	return s;
}

char * itoa(int n, char * s)
{	
	char * p = s;
	
	do
	{
		*p++ = n % 10 + '0';
		n = n / 10;		
	} while (n);
	
	*p = '\0';
	
	reverse(s);
	
	return s;
}
