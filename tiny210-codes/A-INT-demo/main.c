#include "uart.h"
#include "lib.h"
#include "buzzer.h"

void delay(void)
{
	int i = 0;
	
	for (i = 0; i < 0x10000000; i++)
		;
}

void beep(void)
{
	buzzer_on();
	delay();
	buzzer_off();
	delay();
}

void c_irq_handler(void)
{
//	if (STATUS & (1<<16))
	{	
		// clear pending bit	
		*(volatile unsigned int *)0xE0200F48 |= 1<<0;
		
		// clear VIC0ADDRESS  0xF200_0F00 R/W 
		*(volatile unsigned int *)0xF2000F00 = 0;
				
		// call beep
		beep();
	}	
}

extern void IRQ_handler(void);

int mymain(void)
{
	char c = 0x00;
	
	uart_init();
	
	puts("hello, world!\n");

	// Interrupt init 
	// GPH2CON	E020_0C40	1111 = EXT_INT[16] 
	*(int *)0xE0200C40 |= 0xF;
	
	// EXT_INT_2_CON, R/W, Address = 0xE020_0E08
	// 010 = Falling edge triggered
	*(int *)0xE0200E08 |= 1<<1;
	
	// EXT_INT_2_MASK, R/W, Address = 0xE020_0F08
	//*(int *)0xE0200F08 |= 1<<0;		// masked
	*(int *)0xE0200F08 &= ~(1<<0);		// unmasked
	
	// (VICINTENABLE, R/W, Address=0xF200_0010,
	*(int *)0xF2000010 |= (1<<16);		// enable EINT[16]
	
	// VIC0INTSELECT  0xF200_000C R/W 	0:IRQ	1:FIQ
	//*(int *)0xF200000C |= (1<<16);		// enable FIQ
	*(int *)0xF200000C &= ~(1<<16);		// enable IRQ

#if 0	
	// VIC0VECTADDR16  0xF200_0140
	*(int *)0xF2000140 = (int)IRQ_handler;			// ISR Address
	
	// Enable VIC Port
	// 设置协处理器, 汇编完成
	__asm
	{
		mrc p15, 0, r0, c1, c0, 0
		orr r0, r0, #(1<<24)
		mcr p15, 0, r0, c1, c0, 0	
	}
#endif	

	*(int *)0xD0037418 = (int)IRQ_handler;
		
	// enable CPSR I-bit
	__asm
	{
		mov r0, #0x53
		msr CPSR_cxsf, r0
	}
	
	while (1)
	{   
		int pend;
		putchar_hex(c);
		uart_putchar('\r');
		uart_putchar('\n');
		c++;
	
#if 0
		// (EXT_INT_2_PEND, R/W, Address = 0xE020_0F48)
		pend = *(volatile unsigned int *)0xE0200F48;
		if ( pend & (1<<0) )		// EXT_INT_2[0]
		{
			// clear pending bit
			//*(volatile unsigned int *)0xE0200F48 &= ~(1<<0);
			*(volatile unsigned int *)0xE0200F48 |= 1<<0;
			beep();	
		}
#endif
		
#if 0
		// VIC0RAWINTR  0xF200_0008
		pend = *(volatile unsigned int *)0xF2000008;
		if ( pend & (1<<16) )		// EINT[16]
			beep();		
#endif		

		// VIC0IRQSTATUS  0xF200_0000 R  
		// VICFIQSTATUS, R, Address=0xF200_0004, 
		pend = *(volatile unsigned int *)0xF2000000;
		if ( pend & (1<<16) )		// EINT[16]
		{
			// clear pending bit
			*(volatile unsigned int *)0xE0200F48 |= 1<<0;
			beep();	
		}
			
		delay();
	}

	return 0;
}