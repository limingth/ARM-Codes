#define UTRSTAT0  	(*(volatile unsigned int *)0xE2900010)

void uart_init(void);

char uart_getchar(void);

void uart_putchar(char c);
