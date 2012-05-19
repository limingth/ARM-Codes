#include "uart.h"
#include "audio.h"

#define IISTXD		(*(volatile unsigned int *)0xEEE30010)	//IIS TXD DATA
#define IISCON  	(*(volatile unsigned int *)0xEEE30000)	//IIS Control
int mymain(void)
{
	int offset = 0x2E;				// .wav data offset 
	short * p = (short *)0x22000000;		// use "loadb 0x22000000" to put wav file here first
	
	IIC_init();	
	
	WM8960_init();
	
	IIS_init();
	
	uart_putchar('o');
	uart_putchar('k');
	
	while (1)
	{			
	#ifdef DEBUG
		IISTXD = offset++;
		//	delay();
	#else			
		// polling  Primary Tx FIFO0 empty status indication. 
		while((IISCON & (1<<8)) == (1<<8));
		
		IISTXD = *(p+offset);
		
		offset++;
		if (offset > (882046-0x2e) /2)			// 882046 is file size
			offset = 0;			// replay
	#endif	
	}
	
	while (1);

	return 0;
}
