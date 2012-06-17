#define GPA0CON  	(*(volatile unsigned int *)0xE0200000) 

#define ULCON0  	(*(volatile unsigned int *)0xE2900000) 
#define UCON0  		(*(volatile unsigned int *)0xE2900004) 
#define UTRSTAT0  	(*(volatile unsigned int *)0xE2900010)
#define UTXH0  		(*(volatile unsigned char *)0xE2900020) 
#define URXH0  		(*(volatile unsigned char *)0xE2900024) 
#define UBRDIV0 	(*(volatile unsigned int *)0xE2900028) 
#define UDIVSLOT0  	(*(volatile unsigned int *)0xE290002C) 

void uart_init(void)
{
	// set RxD0/TxD0 signal function
	// GPA0_0 	GPA0_1
	//GPA0CON = 0x22;
	GPA0CON &= ~(0xFF<<0);
	GPA0CON |= 0x22;
	
	// set UART SFRs
	ULCON0 = 0x3;
//	UCON0 = 0x245;		// Polling mode
	UCON0 = 0x24A;		// DMA mode
	UBRDIV0 = 0x23;
	UDIVSLOT0 = 0x808;
}

int uart_putchar(char c)
{
	// polling UTRSTAT0	
	//while ((UTRSTAT0 & (1<<2)) == 0)
	while (!(UTRSTAT0 & (1<<2)))
		;

	UTXH0 = c;
	
	return 0;
}

char uart_getchar(void)
{
	char c;
	// polling UTRSTAT0	
	//while ((UTRSTAT0 & (1<<0)) == 0)
	while (!(UTRSTAT0 & (1<<0)))
		;

	c = URXH0;
	
	return c;
}
