
#include "uart.h"

void putchar_hex(char c)
{
	char * hex = "0123456789ABCDEF";
	
	uart_putchar(hex[(c>>4) & 0x0F]);
	uart_putchar(hex[(c>>0) & 0x0F]);
	uart_putchar(' ');
	
	return;
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

void md(char * sdram_addr, int size)
{
	int lines = size / 16;
	int i, j;
	
	for (i = 0; i < lines; i++)
	{
		putint_hex((int)sdram_addr);
		puts(": ");
			
		for (j = 0; j < 16; j++)
			putchar_hex(sdram_addr[i*16+j]);			
		
		puts("\n");
	}
}

void delay(void)
{
	int i;
	
	for (i = 0; i < 0x1000000; i++)
		;
}
