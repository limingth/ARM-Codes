#include "uart.h"
#include "lib.h"
#include "shell.h"
#include "nand.h"

int mymain(void)
{
	char buf[100];
	int argc = 0;
	char * argv[5];
	int i;
	
	uart_init();
	nand_init();
	
	// load superboot and run linux
	// linux(0, (void *)0);
	
	printf("hello, world!\n");
	
	printf("hello, %c%c%c\n", 'a', 'k', 'a');
	printf("hello, %s\n", "edu");
	printf("hello, %x\n", 0x43110);
	printf("hello, %d\n", 0x0);
	printf("hello, %d\n", 12345678);
	
	printf("s5pv210 chip id = 0x%x\n", *(int *)0xE0000000);
	
	while (1)
	{
		puts("\nmyboot> ");
		gets(buf);
		//puts(buf);
		printf("input is <%s> \n", buf);
		
		argc = shell_parse(buf, argv);
		for (i = 0; i < argc; i++)
			printf("argv[%d] = <%s>\n", i, argv[i]);
			
		shell_do(argc, argv);
	}
	
	while (1)
	{
		char c = 'a';
		
		c = uart_getchar();
		
		//for (c = 'a'; c <= 'z'; c++)
		uart_putchar(c);
	}

	return 0;
}