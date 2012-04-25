#include "uart.h"

#define LED		(*(volatile unsigned char *)0x20800000)

#define GPBCON	(*(volatile unsigned int *)0x56000010)
#define GPBDAT	(*(volatile unsigned int *)0x56000014)

#define WTCON	(*(volatile unsigned int *)0x53000000)

void sys_init(void)
{		
	WTCON = 0;
	
	LED = 0x0;
	
	GPBCON = 1;
	
	// beep off
	GPBDAT = 0;
}

int mymain(void)
{	
	char c;

	// system
	sys_init();
	
	// uart init
	uart_init();
	
	// UTXH0 -> data
	while (1)
		//for (c = 'a'; c <= 'z'; c++)
	{	
			c = uart_getchar();
			if (c >= 'a' && c <= 'z')
				uart_putchar(c + 'A' - 'a');
	}
	
	while(1);
	
	return 0;
}