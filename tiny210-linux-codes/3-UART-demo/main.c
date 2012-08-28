//#include "led.h"
#include "uart.h"
#include "lib.h"

int mymain(void)
{
//	led_init();
	uart_init();
	
	uart_putchar('0');
	uart_putchar('x');
	putchar_hex(0xE1);	

	while (1)
	{
		char c = 'a';
		
		c = uart_getchar();
		putchar(c);
		
		for (c = 'a'; c <= 'z'; c++)
			putchar(c);
	}

	return 0;
}
