
#include "uart.h"

void putchar_hex(char c)
{
	char * hex = "0123456789ABCDEF";
	
	uart_putchar(hex[(c>>4) & 0x0F]);
	uart_putchar(hex[(c>>0) & 0x0F]);
	uart_putchar(' ');
	
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

char * gets(char * s)
{
	char c;
	char * p = s;
	
	while ((c = uart_getchar()) != '\r')
	{
		if (c == '\b')
		{
			if (p > s)
			{
				uart_putchar('\b');
				uart_putchar(' ');
				uart_putchar('\b');
				p--;
			}
		}
		else
		{
			uart_putchar(c);
			*p++ = c;
		}
	}
	
	*p = '\0';
	putchar('\n');
	
	return s;
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

int strcmp(const char * s1, const char * s2)
{
	while (*s1 == *s2)
	{
		if (*s1 == '\0')
			return 0;
			
		s1++;
		s2++;
	}

	return (*s1 - *s2);
}

int atoi(const char * s)
{
	int num = 0;
	
	while (*s)
	{
		num = num * 10 + (*s - '0');
		s++;
	}
	
	return num;
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

void md(char * sdram_addr, int size)
{
	int lines = size / 16;
	int i, j;
	
	for (i = 0; i < lines; i++)
	{
		putint_hex((int)sdram_addr + i*16);
		puts(": ");
			
		for (j = 0; j < 16; j++)
			putchar_hex(sdram_addr[i*16+j]);			
		
		puts("\n");
	}
}