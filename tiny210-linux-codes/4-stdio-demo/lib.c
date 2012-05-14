
#include "uart.h"

void delay(void)
{
	int i;
	
	for (i = 0; i < 0x100000; i++)
		;
}

