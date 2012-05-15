#include "uart.h"

#define GPH2CON		(*(volatile unsigned int *)0xE0200C40)
#define GPH2DAT		(*(volatile unsigned int *)0xE0200C44)

// GPIO
#define EXT_INT_2_PEND		(*(volatile unsigned int *)0xE0200F48)
#define EXT_INT_2_CON		(*(volatile unsigned int *)0xE0200E08)
#define EXT_INT_2_MASK		(*(volatile unsigned int *)0xE0200F08)

// VIC
#define VIC0RAWINTR			(*(volatile unsigned int *)0xF2000008)
#define VIC0IRQSTATUS		(*(volatile unsigned int *)0xF2000000)
#define VIC0FIQSTATUS		(*(volatile unsigned int *)0xF2000004)
#define VIC0INTENABLE		(*(volatile unsigned int *)0xF2000010)
#define VIC0INTSELECT		(*(volatile unsigned int *)0xF200000C)
#define VIC0VECTADDR16		(*(volatile unsigned int *)0xF2000140)
  
#define VIC0ADDRESS		(*(volatile unsigned int *)0xF2000F00)
  

void delay(void)
{
	int i;
	
	for (i = 0; i < 0x100000; i++)
		;
}

void C_IRQ_handler(void)
{
	// clear pending bit
	EXT_INT_2_PEND = 1;			
	
	// clear VectAddr
	VIC0ADDRESS = 0;
	
	uart_putchar(' ');
	uart_putchar('+');
	uart_putchar(' ');	
}		

extern void asm_IRQ_handler(void);

int mymain(void)
{
	uart_init();

	// set GPH2_0 as input
	//GPH2CON = 0x0;
	
	// init Interrupt Source
	// 1. Set pin function		1111: EXT_INT[16] 
	GPH2CON = 0xF;	
	
	// 2. Set signaling methed	010 = Falling edge triggered 
	EXT_INT_2_CON = 0x2;	
	
	// 3. Set EXT_INT_2_MASK open
	EXT_INT_2_MASK &= ~(1<<0);
	
	// init VIC (Vectored Interrupt Controller)
	// 1. Set Enable bit (MASK)
	VIC0INTENABLE |= 1<<16;
	
	// 2. Set Mode bit (IRQ/FIQ)
	VIC0INTSELECT &= ~(1<<16);
	
	// init CPSR I-bit (open IRQ disable bit)	0xD3->0x53
	// see it in start.s
	
	// open VIC port 
	// in Cortex A8 core, It seems VIC is open as default 
	
#define VIC	
//#undef VIC
#ifdef VIC
	// install IRQ handler to Vectors
	//VIC0VECTADDR16 = (int)C_IRQ_handler;		// C_IRQ_handler is not RIGHT
	VIC0VECTADDR16 = (int)asm_IRQ_handler;
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
