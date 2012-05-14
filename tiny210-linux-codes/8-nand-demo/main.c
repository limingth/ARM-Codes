#include "led.h"
#include "uart.h"
#include "lib.h"
#include "stdio.h"
#include "shell.h"
#include "command.h"
#include "nand.h"

char buf[64];

int mymain(void)
{
	int argc = 0;
	char * argv[10];
	int i = 0;

	led_init();
	uart_init();
	nand_init();
	nand_read_id(buf);
	putchar_hex(buf[0]);
	putchar_hex(buf[1]);
	putchar_hex(buf[2]);
	putchar_hex(buf[3]);
	putchar_hex(buf[4]);
	
	printf("\n\n" __DATE__ "  " __TIME__ "\n");
	printf("welcome to my boot v1.0 \n\n");
	
	//printf("test: %c,%s,%d,%x\n", 'A', "abcdef", 11, 0x23);

	while (1)
	{
		printf("LUMIT $ ");
		gets(buf);
		
		printf("your input: <%s>\n", buf);
			
		argc = shell_parse(buf, argv);
		
		for (i = 0; i < argc; i++)
			printf("%d: <%s>\n", i, argv[i]);	

		command_do(argc, argv);
	}
}
