#include "lib.h"
#include "stdio.h"
#include "uart.h"
#include "xmodem.h"

void help(int argc, char * argv[])
{
	puts("help: \n");
	puts("md: md 0x0\n");
	puts("mw: mw 0x0 0x1122\n");
	puts("load: load 0x51000000 size\n");
	puts("go: go 0x51000000\n");
	puts("xload: xload 0x51000000\n");
}	

void md(int argc, char * argv[])
{	
	int addr;
	int * p;
	int i = 0; 
	int j = 0;
	
	puts("md usage: md 0x0\n");
	
	addr = atoi(argv[1]);	
	p = (int *)addr;	
	
	for (i = 0; i < 16; i++)
	{
		putx(addr + i*16);
		puts(": ");
		
		for (j = 0; j < 4; j++)
		{
			putx(*p++);
			puts(" ");
		}
		puts("\n");
	}		
}

void mw(int argc, char * argv[])
{	
	int addr;
	int value;
	int * p;
	
	puts("mw usage: mw 0x50000000 0x1122\n");
	
	addr = atoi(argv[1]);	
	value = atoi(argv[2]);
	
	// value -> [addr]
	p = (int *)addr;	
	*p = value;
}


void load(int argc, char * argv[])
{	
	int size = 180;
	int addr = 0x51000000;
	char * p;
	int i;
	
	puts("load usage: load 0x51000000 size\n");
	
	if (argc > 2)
	{
		addr = atoi(argv[1]);
		size = atoi(argv[2]);
	}
	
	puts("Ready for binary download to 0x");
	putx(addr);
	puts(" at 115200 bps...\n");
	
	// get user load addr 
	p = (char *)addr;
	for (i = 0; i < size; i++)
	{
		*p++ = uart_getchar();
	}
	
	puts("load to addr 0x");
	putx(addr);	
	puts("\ntotal size 0x");
	putx(size);
	puts("\nload finished \n");	
}

void xload(int argc, char * argv[])
{		
	int addr = 0x51000000;
	
	puts("xload usage: xload 0x51000000 \n");
	
	if (argc > 1)
	{
		addr = atoi(argv[1]);
	}

	xmodem_recv(addr);
}

void go(int argc, char * argv[])
{		
	int addr = 0x51000000;
	void (*pf)(void);
		
	puts("go usage: go 0x51000000 \n");
	
	if (argc > 1)
	{
		addr = atoi(argv[1]);		
	}
	
	pf = (void (*)(void))addr;
	
	pf();
}



