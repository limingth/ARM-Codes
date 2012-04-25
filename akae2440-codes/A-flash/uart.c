
#define GPHCON	(*(volatile unsigned int *)0x56000070)

#define ULCON0	(*(volatile unsigned int *)0x50000000)
#define UCON0	(*(volatile unsigned int *)0x50000004)
#define UBRDIV0	(*(volatile unsigned int *)0x50000028)
#define UTXH0	(*(volatile unsigned int *)0x50000020)    
#define URXH0	(*(volatile unsigned int *)0x50000024)    
#define UTRSTAT0 (*(volatile unsigned int *)0x50000010)    

#define MPLLCON	(*(volatile unsigned int *)0x4C000004)    
#define CLKDIVN (*(volatile unsigned int *)0x4C000014)
     
void uart_init(void)
{
	// GPH2 TXD[0], GPH3 RXD[0]  
	GPHCON |= 1<<5 | 1<<7;
	
	// Word Length [1:0] 
	// 	11 = 8-bits
	ULCON0 |= 1<<0 | 1<<1;
	
	// polling mode (enable)
	UCON0 |= 1<<0 | 1<<2;
	
	// UBRDIVn  = (int)( UART clock / ( buad rate x 16) ) ¨C1
	// UBRDIVn  = (int)(40000000 / (115200 x 16) ) -1
	// 12000000/19200/16 - 1 = 39.0 -1 = 38
	// 12000000/38400/16 - 1 = 18.5
	// 12000000/57600/16 - 1 = 12
	// 12000000/115200/16 - 1 = 6
	//UBRDIV0 = 38;
	//UBRDIV0 = 19;
	//UBRDIV0 = 12;
	//UBRDIV0 = 6;
	// 50000000/115200/16 - 1 = 26.1
	UBRDIV0 = 26;
		
	// 1:2:6
	CLKDIVN = 1<<0 | 1<<1 | 1<<2;
	
	//  12M -> 304M
	MPLLCON = 0x44<<12 | 1<<4 | 1<<0;
}

void uart_putchar(char c)
{
	// polling status
	while ((UTRSTAT0 & (1<<2)) == 0) 
		;
	
	UTXH0 = c;	
}

char uart_getchar(void)
{
	char c;
	
	// polling status
	while ((UTRSTAT0 & (1<<0)) == 0) 
		;
	
	c = URXH0;	
	
	return c;
}