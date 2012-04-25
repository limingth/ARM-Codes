#include "uart.h"
#include "stdio.h"

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
	char buf[100];

	// system
	sys_init();
	
	// uart init
	uart_init();
	
	//puts("hello, world! \n");
	
	puts("\n\n" __DATE__ "  " __TIME__ "\n");
	puts("welcome to my boot v1.0 \n");
	
	// UTXH0 -> data
	while (1)
	{
		puts("akaedu $ ");
		gets(buf);
		puts("your input:");
		puts(buf);
		puts("\n");			
	}
	
	while(1);
	
	return 0;
}