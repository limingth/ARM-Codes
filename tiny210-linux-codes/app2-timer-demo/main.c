#include "uart.h"
#include "buzzer.h"

#define GPH2CON		(*(volatile unsigned int *)0xE0200C40)
#define GPH2DAT		(*(volatile unsigned int *)0xE0200C44)

// Timer
#define TCFG0		(*(volatile unsigned int *)0xE2500000)
#define TCFG1		(*(volatile unsigned int *)0xE2500004)
#define TCON		(*(volatile unsigned int *)0xE2500008)
#define TCNTB0		(*(volatile unsigned int *)0xE250000C)
#define TCMPB0		(*(volatile unsigned int *)0xE2500010)
#define TCNTO0		(*(volatile unsigned int *)0xE2500014)
#define TINT_CSTAT	(*(volatile unsigned int *)0xE2500044)
   
// VIC
#define VIC0RAWINTR		(*(volatile unsigned int *)0xF2000008)
#define VIC0IRQSTATUS		(*(volatile unsigned int *)0xF2000000)
#define VIC0FIQSTATUS		(*(volatile unsigned int *)0xF2000004)
#define VIC0INTENABLE		(*(volatile unsigned int *)0xF2000010)
#define VIC0INTSELECT		(*(volatile unsigned int *)0xF200000C)
#define VIC0VECTADDR16		(*(volatile unsigned int *)0xF2000140)
#define VIC0VECTADDR21		(*(volatile unsigned int *)0xF2000154)

#define VIC0ADDRESS		(*(volatile unsigned int *)0xF2000F00)
  

void delay(void)
{
	int i;
	
	for (i = 0; i < 0x10000; i++)
		;
}

void beep(void)
{
	buzzer_on();
	delay();
	buzzer_off();
	delay();
}

void C_IRQ_handler(void)
{
	// clear pending bit	
	TINT_CSTAT |= 1<<5;
 				
	// clear VIC0ADDRESS  0xF200_0F00 R/W 
 	VIC0ADDRESS = 0;

	uart_putchar(' ');
	uart_putchar('+');
	uart_putchar(' ');

	// call beep
	beep();	
}		

extern void asm_IRQ_handler(void);

int mymain(void)
{
	buzzer_init();
	uart_init();

	// Interrupt init 
	// INT Source init
	// PCLK / (65+1) = 1M
	TCFG0 = 65;
	
	// 1M/16 = 62500hz
	TCFG1 = 0x4;
	
	// 1s = 62500hz
	TCNTB0 = 62500;
	
	// Set the manual update bit 
	TCON |= 1<<1;
	
	// and clear only manual update bit 
	TCON &= ~(1<<1);
		
	// step 3: Set the start bit 
	TCON |= 1<<0;

	// set auto-reload bit
	TCON |= 1<<3;		
		
	// step 4: Enable interrupt TINT_CSTAT bit[0] 
	TINT_CSTAT |= 1<<0;
	
	// VIC init
	// (VICINTENABLE, R/W, Address=0xF200_0010,
	VIC0INTENABLE |= (1<<21);		// enable EINT[21]
	
	// VIC0INTSELECT  0xF200_000C R/W 	0:IRQ	1:FIQ
	//*(int *)0xF200000C |= (1<<21);		// enable FIQ
	VIC0INTSELECT &= ~(1<<21);		// enable IRQ

	// init CPSR I-bit (open IRQ disable bit)	0xD3->0x53
	// see it in start.s
	
	// open VIC port 
	// in Cortex A8 core, It seems VIC is open as default 
	
#define VIC	
//#undef VIC
#ifdef VIC
	// install IRQ handler to Vectors	
	VIC0VECTADDR21 = (int)asm_IRQ_handler;
#else
	// set all exception vectors addresses from 0xD0037400
	*(int *)(0xD0037400 + 0x18) = (int)asm_IRQ_handler;	
#endif
	
	while (1)
	{	
		char c;
		
		for (c = 'a'; c <= 'z'; c++)
		{
			uart_putchar(c);
			delay();
		}		
	}	
		
	return 0;
}
