
#define GPHCON	(*(volatile unsigned int *)0x56000070)
#define UCON0	(*(volatile unsigned int *)0x50000004)
#define UBRDIV0	(*(volatile unsigned int *)0x50000028)
#define ULCON0	(*(volatile unsigned int *)0x50000000)

#define UTRSTAT0	(*(volatile unsigned int *)0x50000010)
#define UTXH0	(*(volatile unsigned int *)0x50000020)
#define URXH0	(*(volatile unsigned int *)0x50000024)

void uart_init(void)
{			
	// uart init
	// GPH2, GPH3 function select = TXD0, RXD0
	GPHCON = 1<<7 | 1<<5;
	//GPHCON = 1<<5;
	
	// enable Txd, Rxd
	// 01 = Interrupt request or polling mode
	UCON0 = 1<<0 | 1<<2;
	//UCON0 = 1<<2;
	
	// PCLK = Fin (12Mhz)
	// UBRDIVn = (int)(PCLK/(bps x 16)) ¨C 1
	// 12M = 12000000 / 19200 / 16 - 1
	// = 39.0625 - 1 = 38
	UBRDIV0 = 38;
	
	// 11 = 8-bits
	ULCON0 = 1<<0 | 1<<1;
}

void uart_putchar(char c)
{
	// check Transmitter empty == 1
	while( (UTRSTAT0 & (1<<2)) == 0 )
		;
			
	UTXH0 = c;
}


char uart_getchar(void)
{
	char c;
	
	// check Receive buffer empty == 0
	while( (UTRSTAT0 & (1<<0)) == 0 )
		;
			
	c = URXH0;
	
	return c;
}
