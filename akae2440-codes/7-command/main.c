#include "uart.h"
#include "stdio.h"
#include "shell.h"
#include "command.h"

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
	int argc = 0;
	char * argv[10];
	int i = 0;
	
	// system
	sys_init();
	
	// uart init
	uart_init();
	
	
	puts("\n\n" __DATE__ "  " __TIME__ "\n");
	puts("welcome to my boot v1.0 \n");
	
	printf("test: %c,%s,%d,%x\n", 'A', "abcdef", 11, 0x23);
		
	// UTXH0 -> data
	while (1)
	{
		puts("akaedu $ ");
		gets(buf);
		
		printf("your input: <%s>\n", buf);
			
		argc = shell_parse(buf, argv);
		
		for (i = 0; i < argc; i++)
			printf("%d: <%s>\n", i, argv[i]);	
						
		command_do(argc, argv);		
	}
	
	while(1);
	
	return 0;
}