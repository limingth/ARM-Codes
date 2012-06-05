#include "sdhc.h"
#include "uart.h"
#include "stdio.h"

void printbuf(char * buff, int size)
{
	int * p;
	int j;

	printf("\n");
	p = (int *)buff;
	for (j = 0; j < size/4; j++)
	{
		printf("%x ", *(p+j));
		
		if ( j % 4 == 3 )
			printf("\n");
	}
	printf("\n");
}

int mymain(void)
{
	char buf[512];
	
	puts("sd init begin\n");
//	uart_init();
	SDHC_Init();
	puts("sd init over\n");

	SDHC_ReadBlocks(0x1, 1, (unsigned int)buf);
	printbuf(buf, 512);
	SDHC_ReadBlocks(0x1dc000, 1, (unsigned int)buf);
	printbuf(buf, 512);
	
	return 0;
}
