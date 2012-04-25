#include "uart.h"
#include "lib.h"
#include "buzzer.h"

void delay(void)
{
	int i = 0;
	
	for (i = 0; i < 0x1000000; i++)
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
		*(volatile unsigned int *)0xE2500044 |= 1<<5;
		
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
	// INT Source init
	// PCLK / (65+1) = 1M
	*(int *)0xE2500000 = 65;
	
	// 1M/16 = 62500hz
	*(int *)0xE2500004 = 0x4;
	
	// 1s = 62500hz
	*(int *)0xE250000C = 62500;
	
	// Set the manual update bit 
	*(volatile unsigned int *)0xE2500008 |= 1<<1;
	
	// and clear only manual update bit 
	*(volatile unsigned int *)0xE2500008 &= ~(1<<1);
		
	// step 3: Set the start bit 
	*(volatile unsigned int *)0xE2500008 |= 1<<0;

	// set auto-reload bit
	*(volatile unsigned int *)0xE2500008 |= 1<<3;		
		// step 4: Enable interrupt TINT_CSTAT bit[0] 
	*(volatile unsigned int *)0xE2500044 |= 1<<0;
	
#if 1
	// VIC init
	// (VICINTENABLE, R/W, Address=0xF200_0010,
	*(int *)0xF2000010 |= (1<<21);		// enable EINT[21]
	
	// VIC0INTSELECT  0xF200_000C R/W 	0:IRQ	1:FIQ
	//*(int *)0xF200000C |= (1<<21);		// enable FIQ
	*(int *)0xF200000C &= ~(1<<21);		// enable IRQ
#endif

#if 1
	// ARM CORE init: enable CPSR I-bit
	__asm
	{
		mov r0, #0x53
		msr CPSR_cxsf, r0
	}
#endif

#if 1
	// VIC0VECTADDR21  0xF200_0140
	*(int *)0xF2000154 = (int)IRQ_handler;			// ISR Address
	
	// Enable VIC Port
	// 设置协处理器, 汇编完成
	__asm
	{
		mrc p15, 0, r0, c1, c0, 0
		orr r0, r0, #(1<<24)
		mcr p15, 0, r0, c1, c0, 0	
	}
#endif	
	
	while (1);
	
	{   
		int pend;
		int cnt;
		
		cnt = *(volatile unsigned int *)0xE2500014;
		putint_hex(cnt);		
		uart_putchar('\r');
		uart_putchar('\n');	
	
#if 0
		// 
		pend = *(volatile unsigned int *)0xE2500044;
		if ( pend & (1<<5) )		// bit[5] = Timer0 int status
		{
			// clear pending bit, set TINT_CSTAT bit[5] write '1' to clear			
			*(volatile unsigned int *)0xE2500044 |= 1<<5;
			beep();	
		}
#endif
		
#if 0
		// VIC0RAWINTR  0xF200_0008
		pend = *(volatile unsigned int *)0xF2000008;
		if ( pend & (1<<21) )		// EINT[21] = Timer0
		{
			// clear pending bit, set TINT_CSTAT bit[5] write '1' to clear			
			*(volatile unsigned int *)0xE2500044 |= 1<<5;
			beep();	
		}
#endif		

#if 1
		// VIC0IRQSTATUS  0xF200_0000 R  
		// VICFIQSTATUS, R, Address=0xF200_0004, 
		pend = *(volatile unsigned int *)0xF2000000;
		if ( pend & (1<<21) )		// EINT[21]
		{
			// clear pending bit
			*(volatile unsigned int *)0xE2500044 |= 1<<5;
			beep();	
		}
#endif		
			
		delay();
	}

	return 0;
}