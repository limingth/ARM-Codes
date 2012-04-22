#include "stdio.h"
#include "lib.h"

void help(int argc, char * argv[])
{
	printf("help: \n");
	printf("md: md 0x0\n");
	printf("mw: mw 0x0 0x1122\n");
	printf("load: load 0x31000000\n");
	printf("go: go 0x31000000\n");
}	

void md(int argc, char * argv[])
{	
	int addr;
	int * p;
	int i = 0; 
	int j = 0;
	
	printf("md usage: md 0x0\n");
	
	addr = atoi(argv[1]);
	p = (int *)addr;	
	
	for (i = 0; i < 16; i++)
	{		
		printf("%x:", addr + i*16);
		
		for (j = 0; j < 4; j++)
			printf(" %x", *p++);

		printf("\n");
	}		
}

void mw(int argc, char * argv[])
{	
	int addr;
	int value;
	int * p;
	
	printf("mw usage: mw 0x30000000 0xFF\n");
	
	addr = atoi(argv[1]);	
	value = atoi(argv[2]);
	
	// value -> [addr]
	p = (int *)addr;	
	*p = value;
}
	