#include "uart.h"

int strcmp(const char * s1, const char * s2)
{
	while( *s1 != '\0' && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	
	if (*s1 == '\0' && *s2 == '\0')
		return 0;
		
	return (*s1-*s2);
}


int atoi(char * str)
{
	int i;
	int value = 0;
	int c_value;
	char c;
	
	// deal with base(16)
	if (str[0] == '0' && str[1] == 'x')
	{
		i = 2;
		while ((c = str[i++]) != '\0')
		{
			c_value = (c >= '0' && c <= '9' ? c - '0': c - 'a' + 10);
			value = value * 16 + c_value;
		}
		return value;
	}

	// deal with base(10)
	i = 0;
	while ((c = str[i++]) != '\0')
	{
		c_value =  c - '0';
		value = value * 10 + c_value;
	}
	
	return value;
}

void puthex(char c)
{
	static char hex[17] = "0123456789ABCDEF";
	
	uart_putchar(hex[c]);
}

void putcharx(char c)
{
	unsigned char c0, c1;
	
	c0 = c & 0x0F;
	c1 = (c & 0xF0) >> 4;
	
	puthex(c1);
	puthex(c0);	
}

void putx(int value)
{
	unsigned char c0, c1, c2, c3;
	c0 = value & 0xFF;
	c1 = (value & 0xFF00) >> 8;
	c2 = (value & 0xFF0000) >> 16;
	c3 = (value & 0xFF000000) >> 24;
	
	putcharx(c3);
	putcharx(c2);
	putcharx(c1);
	putcharx(c0);
}