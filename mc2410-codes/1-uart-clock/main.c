#include "uart.h"


#define MPLLCON	(*(volatile unsigned int *)(0x4C000004))
#define UPLLCON	(*(volatile unsigned int *)(0x4C000008))
#define CLKCON	(*(volatile unsigned int *)(0x4C00000C))
#define CLKDIVN	(*(volatile unsigned int *)(0x4C000014))

#define UBRDIV0	(*(volatile unsigned int *)0x50000010)

#define WTCON 	(*(volatile unsigned int *)0x53000000)

void delay(void)
{
	int i;
	
	for (i = 0; i < 100000; i++)
		;	
}

int mymain(void)
{
	char c = 'A';

	// disable watchdog WTCON (refer to S3C2440_ref.pdf page462)
	WTCON = 0;
	
#if 0
vivi> dump 0x4c000000 100
4C000000: FF FF FF 00 40 C0 05 00-32 80 04 00 F0 FF 0F 00 | ....@...2.......
4C000010: 04 00 00 00 03 00 00 00-00 00 00 00 00 00 00 00 | ................
4C000020: FF FF FF 00 40 C0 05 00-32 80 04 00 F0 FF 0F 00 | ....@...2.......
4C000030: 04 00 00 00 03 00 00 00-00 00 00 00 00 00 00 00 | ................
4C000040: FF FF FF 00 40 C0 05 00-32 80 04 00 F0 FF 0F 00 | ....@...2.......
4C000050: 04 00 00 00 03 00 00 00-00 00 00 00 00 00 00 00 | ................
4C000060: FF FF FF 00      

vivi> dump 0x50000000 100
50000000: 03 00 00 00 45 02 00 00-00 00 00 00 00 00 00 00 | ....E...........
50000010: 02 00 00 00 00 00 00 00-00 00 00 00 00 00 00 00 | ................
50000020: 00 00 00 00 0D 00 00 00-1A 00 00 00 00 00 00 00 | ................
50000030: 00 00 00 00 00 00 00 00-00 00 00 00 00 00 00 00 | ................
50000040: 00 00 00 00 00 00 00 00-00 00 00 00 00 00 00 00 | ................
50000050: 00 00 00 00 00 00 00 00-00 00 00 00 00 00 00 00 | ................
50000060: 00 00 00 00                                     | ....            
vivi> 
	
#endif		
	CLKDIVN = 0x1<<0 | 0x1<<1;
	
	// 0005c040
	// (92+8) * 12 / (4+2) * 2^0 
	// 100 * 12M / 6 * 1 = 200M	
	MPLLCON = 0x0005C040;	
			
	//delay();
		
	// UART init
	uart_init();
		
	while(1)
	{
	//	for (c = 'A'; c <= 'D'; c++)
		{				
			//c = uart_getchar();
			uart_putchar(c);	
		}	
	}
	return 0;
}