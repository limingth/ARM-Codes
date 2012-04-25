#include "led.h"
#include "uart.h"
#include "stdio.h"
#include "shell.h"
#include <string.h>
#include <stdlib.h>
#include "command.h"

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
		if (strcmp(argv[0], "help") == 0)
			help(argc, argv);
		
		if (strcmp(argv[0], "md") == 0)
			md(argc, argv);	
		
		if (strcmp(argv[0], "mw") == 0)
			mw(argc, argv);	
			
		if (strcmp(argv[0], "load") == 0)
			load(argc, argv);	
			
		if (strcmp(argv[0], "go") == 0)
			go(argc, argv);				
	}
	
	return 0;
}