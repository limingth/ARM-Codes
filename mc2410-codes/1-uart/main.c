#include "uart.h"

int mymain(void)
{
	char c;

	// UART init
	//uart_init();

	while(1)
	//	for (c = 'A'; c <= 'Z'; c++)
		{			
			uart_putchar('$');
			c = uart_getchar();
			uart_putchar(c);		
		}	
	
	return 0;
}