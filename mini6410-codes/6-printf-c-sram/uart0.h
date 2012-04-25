/*
 *    uart0.h  -  header file of dip 
 *
 *	  Board:		akae2440
 *    environment:	bootloader & ADS
 *    Author:		akaedu
 *    Date:			2009-5-26
 *    web:			www.akaedu.org
 *
 */
 
 int uart0_init(void);
 char uart0_getchar(void);
 int uart0_putchar(char ch);
 int uart0_test(void);