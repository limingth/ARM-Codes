#include "uart.h"
//#include <stdio.h>
#include "stdio.h"

#define MPLLCON	(*(volatile unsigned int *)(0x4C000004))
#define UPLLCON	(*(volatile unsigned int *)(0x4C000008))
#define CLKCON	(*(volatile unsigned int *)(0x4C00000C))
#define CLKDIVN	(*(volatile unsigned int *)(0x4C000014))

#define WTCON	(*(volatile unsigned int *)(0x53000000))

void sys_init()
{
	// watch dog disable
	WTCON = 0x0;
	
	// 1:2:4	
	// PCLK:HCLK:FCLK = 50M:100M:200M
	CLKDIVN = 0x1<<0 | 0x1<<1;

	MPLLCON = 0x0005C040;
}

int mymain(void)
{
	char buf[100];
	
	// clock init, watch dog disable
	sys_init();
		
	// UART init
	uart_init();

	printf("\n\nwelcome to my boot v1.0 \n");
	printf("Date is %s\n", __DATE__);
	printf("Version is 0x%x\n", 0x87654321);
	
	while(1)	
	{	
		printf("Jobs $ ");
		gets(buf);	
		printf("user input: <%s>\n", buf);
	}
	
	return 0;
}