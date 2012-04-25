#include "led.h"
#include "uart.h"
#include "stdio.h"
#include "shell.h"

void help(void)
{
	puts("help: \n");
	puts("md: md 0x0\n");
	puts("mw: mw 0x0 0x1122\n");
	puts("load: load 0x51000000\n");
	puts("go: go 0x51000000\n");
}	
	
int mymain(void)
{	
	char buf[100];
	int argc = 0;
	char * argv[4];
	int i = 0;
	
	led_init();	
	
	// uart init
	uart_init();
	
	puts("\nWelcome to my bootloader v1.0 (2011-10-6)\n");
	
	while(1)
	{	
		puts("Jobs $ ");
		gets(buf);
		puts(buf);
		puts("\r\n");
		
		// shell parse buf -> argc, argv[]
		shell_parse(buf, &argc, argv);
		
		for (i = 0; i < argc; i++)
		{
			puts(argv[i]);
			puts("\r\n");
		}
		
		// command: help
		if (argv[0][0] == 'h')
			help();
	}
	
	return 0;
}