#define GPA0CON  	(*(volatile unsigned int *)0xE0200000) 

#define ULCON0  	(*(volatile unsigned int *)0xE2900000) 
#define UCON0  		(*(volatile unsigned int *)0xE2900004) 
#define UTRSTAT0  	(*(volatile unsigned int *)0xE2900010)
#define UTXH0  		(*(volatile unsigned char *)0xE2900020) 
#define URXH0  		(*(volatile unsigned char *)0xE2900024) 
#define UBRDIV0 	(*(volatile unsigned int *)0xE2900028) 
#define UDIVSLOT0  	(*(volatile unsigned int *)0xE290002C)

#define APLL_CON  	(*(volatile unsigned int *)0xe0100100) 
#define CLK_SRC0  	(*(volatile unsigned int *)0xe0100200) 
#define CLK_DIV0  	(*(volatile unsigned int *)0xe0100300) 
#define MPLL_CON  	(*(volatile unsigned int *)0xe0100108)  

void uart_init(void)
{
#if 1
	// clock init
	APLL_CON = 0xa07d0301;
	CLK_SRC0 = 0x10001111;
	CLK_DIV0 = 0x14131440;
	MPLL_CON = 0xa29b0c01;
#endif

	// 66Mhz / (115200*16) - 1 = 0x23
	// 66Mhz / (19200*16) - 1 = 0xD5
	GPA0CON &= ~(0xFF<<0);
	GPA0CON |= 0x22;
			
	// set UART SFRs
	ULCON0 = 0x3;
	UCON0 = 0x245;
	UBRDIV0 = 0x23;
	UDIVSLOT0 = 0x808;

	return;
}

char uart_getchar(void)
{
	char c;
	// polling receive status: if buffer is full
	//while ((UTRSTAT0 & (1<<0)) == 0)
	while (!(UTRSTAT0 & (1<<0)))
		;
	
	c = URXH0;
		
	return c;
}

void uart_putchar(char c)
{
	// polling transmit status: if buffer is empty
	//while ((UTRSTAT0 & (1<<2)) == 0)
	while (!(UTRSTAT0 & (1<<2)))
		;
	
	UTXH0 = c;
	
	return;
}
