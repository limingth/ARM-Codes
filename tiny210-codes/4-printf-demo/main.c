#include "uart.h"
#include "lib.h"

int mymain(void)
{
	uart_init();
	
	myprintf("hello, world!\n");
	
	myprintf("hello, %c%c%c\n", 'a', 'k', 'a');
	myprintf("hello, %s\n", "edu");
	myprintf("hello, %x\n", 0x43110);
	myprintf("hello, %d\n", 0x0);
	myprintf("hello, %d\n", 12345678);
	
	myprintf("s5pv210 chip id = 0x%x\n", *(int *)0xE0000000);
	
	while (1)
	{
		char c = 'a';
		
		c = uart_getchar();
		
		//for (c = 'a'; c <= 'z'; c++)
		uart_putchar(c);
	}

	return 0;
}