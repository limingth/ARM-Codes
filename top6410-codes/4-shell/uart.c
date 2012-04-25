
#include "uart.h"

#define UCON0	(*(volatile unsigned int *)0x7F005004)
#define UBRDIV0	(*(volatile unsigned int *)0x7F005028)
#define ULCON0	(*(volatile unsigned int *)0x7F005000)
#define UTRSTAT0 (*(volatile unsigned int *)0x7F005010)
#define UTXH0 	(*(volatile unsigned int *)0x7F005020)
#define URXH0 	(*(volatile unsigned int *)0x7F005024)
	
void uart_init(void)
{
	// UCON0 Transmit Mode [3:2] 
	// 01 = Interrupt request or polling mode 
	UCON0 |= 1<<2;
	UCON0 &= ~(1<<3);
	
	// UCON0 Receive Mode [1:0] 
	// 01 = Interrupt request or polling mode 
	UCON0 |= 1<<0;
	UCON0 &= ~(1<<1);
	
	// Clock Selection [11:10] 
	// 11 = EXT_UCLK1:   DIV_VAL = (EXT_UCLK1 / (bps x 16) ) ¨C1 
	UCON0 |= 1<<10 | 1<<11;
	
	// UART BAUD RATE 
	// DIV_VAL = (EXT_UCLK1 / (bps x 16 ) ) - 1 
	// 66M / (115200*16) - 1 = 35.8 - 1 = 34.8
	UBRDIV0 = 34;	
	
	// UART channel 0 line control register 
	// Word Length [1:0] 11 = 8-bit
	ULCON0 |= 1<<0 | 1<<1;
}

void uart_putchar(char c)
{
	// check if empty?
	while( (UTRSTAT0 & (1<<2)) == 0 )
		;
			
	UTXH0 = c;
}

char uart_getchar(void)
{
	char c;
	
	// check if receive buffer is full?
	while( (UTRSTAT0 & (1<<0)) == 0 )
		;	
	
	c = URXH0;
	
	return c;
}