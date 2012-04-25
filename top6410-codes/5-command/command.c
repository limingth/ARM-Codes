#include "lib.h"
#include "stdio.h"

void help(int argc, char * argv[])
{
	puts("help: \n");
	puts("md: md 0x0\n");
	puts("mw: mw 0x0 0x1122\n");
	puts("load: load 0x51000000\n");
	puts("go: go 0x51000000\n");
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
	