#include "stdio.h"

#define NAK 0x15
#define EOT 0x04
#define ACK 0x06

void delay()
{
	int i;
	for(i = 0; i < 0x200000; i++);
}

void xmodem_recv(char *addr)
{
	char c;
	int i;
	for (c = '9'; c > '0'; c--)
	{
		putchar(c);
		delay();
	}
	
	putchar(NAK);
	while (getchar() != EOT)
	{
		getchar(); 
		getchar();
		for (i = 0; i < 128; i++)
			*addr++ = getchar();
		getchar();
		putchar(ACK);
	}
	putchar(ACK);
}