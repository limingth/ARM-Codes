#include "uart.h"
#include "stdio.h"
#include "command.h"
#include "nand.h"
//#include <stdio.h>

#define WTCON 	(*(volatile int *)0x53000000)
	
#define LED		(*(volatile unsigned char *)0x20800000)

#define GPBCON		(*(volatile unsigned int *)0x56000010)
#define GPBDAT		(*(volatile unsigned int *)0x56000014)

#define MPLLCON		(*(volatile unsigned int *)0x4C000004)
#define CLKDIVN 	(*(volatile unsigned int *)0x4c000014)
#define UTXH0 		(*(volatile unsigned int *)0x50000020)
#define UBRDIV0 	(*(volatile unsigned int *)0x50000028)

#define BWSCON 		(*(volatile unsigned int *)0x48000000)
#define BANKCON6	(*(volatile unsigned int *)0x4800001C)
#define BANKSIZE	(*(volatile unsigned int *)0x48000028)
#define REFRESH		(*(volatile unsigned int *)0x48000024)

#define UTRSTAT0 *(volatile int *)0x50000010
#define URXH0 *(volatile int *)0x50000024
	
void sys_init(void)
{
	WTCON = 0;//disable watch dog
	
	GPBCON |= 1<<0;//disable beeper
	GPBCON &= ~(1<<1);	
	GPBDAT = 0x0;
}

void clock_init(void)
{
#ifdef S3C2440
	MPLLCON = 0x44011;
	CLKDIVN = 0x7;
#endif
	
	// 1:2:4	
	// PCLK:HCLK:FCLK = 50M:100M:200M
	CLKDIVN = 0x1<<0 | 0x1<<1;

	MPLLCON = 0x0005C040;	
	
	//UBRDIV0 = 325;	
	UBRDIV0 = 26;		// 115200 baudrate	
}

void sdram_init(void)
{
	// DW6 [25:24]	10 = 32-bit  
	BWSCON |= 1<<25;
	
	// SCAN [1:0] Column address number		01 = 9-bit 
	BANKCON6 |= 1<<0;

	// BK76MAP [2:0] BANK6/7 memory map		001 = 64MB/64MB
	BANKSIZE = 0x1;
	
	// refresh period is 7.8 us and HCLK is 100MHz
	// Refresh count = 211 + 1 - 100x7.8 = 1269
	// 7.8 us = 64ms / 8192(8K) = 7.8125
	REFRESH |= 1269;
}

#if 0
int shell_parse2(char * buf, char * argv[])
{
	int argc = 0;
	int state = 0;
	int input = 0;
	
	while (*buf)
	{
		//input = get_input_type(*buf);
		if (*buf == ' ')
			input = 0;
		else
			input = 1;
		
		switch (state)
		{
			case 0:
				switch (input)
				{
					case 0: // ' '						
						break;
					case 1: // abc
						argv[argc++] = buf;
						state = 1;
						break;
				}
				break;
			case 1:	
				switch (input)
				{
					case 0: // ' '
						*buf = '\0';
						state = 0;
						break;
					case 1: // abc					
						break;
				}	
				break;
		}	
		buf++;
	}

	return argc;
}
#endif

int shell_parse(char * buf, char * argv[])
{
	int argc = 0;
	int state = 0;

	while (*buf != '\0')
	{
		if (*buf != ' ' && state == 0)
		{
			argv[argc++] = buf;
			state = 1;
		}
		
		if (*buf == ' ' && state == 1)
		{
			*buf = '\0';
			state = 0;
		}
			
		buf++;
	}
	
	return argc;
}

void autoplay(void);

int mymain()
{	
	char buf[32];
	int argc;
	char * argv[5];
	int i = 0;
	int userinput = 0;
	
	sys_init();
	uart_init();	
	clock_init();
	sdram_init();
	nand_init();
		
	puts("\n\nWelcome to my boot 1.0! \n");
	
//	printf("buf %c is %s, %d, %x %x \n", 'A', "abcd", 100, 101, 100);
	
	printf("please input Enter to Shell mode\n");
	for (i = 0x300000; i > 0; i--)
	{
		if ((i & 0xfffff) == 0) 
			printf("\rtime left %d", i>>20);
	
		if (UTRSTAT0 & (1<<0))
		{
			userinput = URXH0;	// no use
			userinput = 1;			
			break;
		}
	}
	printf("\n");
	
	// judge if user input ENTER
	if (userinput != 1)
	{
		// autoplay
		//printf("autoplay begin... \n");
		//autoplay();
		printf("autorun begin... \n");
		autoplay();
	}
	
	while(1)
	{
		puts("myboot> ");
		gets(buf);
		//printf("input command is <%s> \n", buf);
		
		argc = shell_parse(buf, argv);
		//for (i = 0; i < argc; i++)
		//	printf("argv[%d] is <%s> \n", i, argv[i]);
			
		//printf("command is <%s> \n", argv[0]);
		command_do_main(argc, argv);
	}
}