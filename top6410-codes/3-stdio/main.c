#include "led.h"
#include "uart.h"
#include "stdio.h"

int mymain(void)
{	
	char buf[100];
	
	led_init();	
	
	// uart init
	uart_init();
	
	puts("Welcome to my bootloader v1.0 (2011-10-6)\n");
	
	while(1)
	{	
		puts("Jobs $ ");
		gets(buf);
		puts(buf);
		puts("\r\n");
	}
	
	return 0;
}