#define ULCON0 *(volatile int *)0x50000000
#define UCON0 *(volatile int *)0x50000004
#define UTRSTAT0 *(volatile int *)0x50000010
#define UTXH0 *(volatile int *)0x50000020
#define URXH0 *(volatile int *)0x50000024
#define UBRDIV0 *(volatile int *)0x50000028
#define GPHCON *(volatile int *)0x56000070

void uart_init(void)
{
	GPHCON |= 1<<5;//set GPIO to TXD RXD
	GPHCON |= 1<<7;
	GPHCON &= ~(1<<4);
	GPHCON &= ~(1<<6);
	
	ULCON0 = 0x3;//set uart frame format
	UCON0 = 0x5;//enable transmit and receive
	UBRDIV0 = 77;//set baud rate
}

void uart_putchar(char c)
{
	while((UTRSTAT0 & (1<<2)) == 0);
	UTXH0 = c;
}

char uart_getchar(void)
{
	while((UTRSTAT0 & (1<<0)) == 0);
	return URXH0;
}

