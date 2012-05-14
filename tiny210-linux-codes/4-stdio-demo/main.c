#include "led.h"
#include "uart.h"
#include "lib.h"
#include "stdio.h"

int mymain(void)
{
	char buf[64];

	led_init();
	uart_init();
	
	puts("\n\n" __DATE__ "  " __TIME__ "\n");
	puts("welcome to my boot v1.0 \n");
	
	printf("test: %c,%s,%d,%x\n", 'A', "abcdef", 11, 0x23);
		
	while (1)
	{
		puts("LUMIT $ ");
		gets(buf);
		
		printf("your input: <%s>\n", buf);
	}
}
