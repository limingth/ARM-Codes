#include "uart.h"
#include "lib.h"

int mymain(void)
{
	uart_init();
	
	puts("hello, world!\n");
	
	uart_putchar('0');
	uart_putchar('x');
	putchar_hex(0xE1);	
	
	while (1)
	{
		char c = 'a';
		
		c = uart_getchar();
		
		//for (c = 'a'; c <= 'z'; c++)
		uart_putchar(c);
	}

	return 0;
}