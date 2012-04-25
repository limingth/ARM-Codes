#include "led.h"
#include "uart.h"

void delay(void)
{
	int i;
	for (i = 0; i < 1000000; i++)
		;
}
	
int mymain(void)
{	
	int * p = (int *)0x7f005000;
	int i = 0;
	char c;
	
	led_init();	
	
	// clear all uart0 regs
	for (i = 0; i < 12; i++)
		*(p+i) = 0x0;
	
	// uart init
	uart_init();
	
	for (c = 'a'; c <= 'z'; c++)
		uart_putchar(c);
	
	while(1)
	{		
		c = uart_getchar();		
		uart_putchar(c);
	#if 0	
		led_on();
		delay();			
		led_off();
		delay();
	#endif	
	}
	
	return 0;
}