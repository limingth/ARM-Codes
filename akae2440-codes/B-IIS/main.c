#include "uart.h"
#include "stdio.h"
#include "shell.h"
#include "command.h"

#define LED		(*(volatile unsigned char *)0x20800000)

#define GPBCON	(*(volatile unsigned int *)0x56000010)
#define GPBDAT	(*(volatile unsigned int *)0x56000014)

#define WTCON	(*(volatile unsigned int *)0x53000000)
#define BWSCON	(*(volatile unsigned int *)0x48000000)

#define UTRSTAT0 (*(volatile unsigned int *)0x50000010)

void sys_init(void)
{		
	WTCON = 0;
	
	// sdram init: 0x30000000 - 0x34000000 (64M SDRAM)
	BWSCON |= 1<<25;
	
	LED = 0x0;
	
	GPBCON = 1;
	
	// beep off
	GPBDAT = 0;
}

void autorun(void)
{		
	char * nand_argv[4] = {"nandread", "0x100000", "0x32000000", "0x100000"};				
//	char * go_argv[2] = {"go", "0xc00"};
	char * play_argv[2] = {"play", "0x32000000"};
	
	nandread(4, nand_argv);
//	go(2, go_argv);
	play(2, play_argv);	
}

void loadwav(void)
{		
	char * nand_argv[4] = {"nandread", "0x100000", "0x32000000", "0x100000"};				
//	char * go_argv[2] = {"go", "0xc00"};
	
	nandread(4, nand_argv);
//	go(2, go_argv);
}		
		
int mymain(void)
{	
	char buf[100];
	int argc = 0;
	char * argv[10];
	int i = 0;
	int flag = 0;
	
	// system
//	sys_init();
	
	// uart init
	uart_init();
		
	puts("\n\n" __DATE__ "  " __TIME__ "\n");
	puts("welcome to my boot v1.0 \n");
	
//	printf("test: %c,%s,%d,%x\n", 'A', "abcdef", 11, 0x23);
	
	printf("please enter a key to shell mode in 3 seconds... \n");
	
	while (i < 0x400000)
	{
		if ((UTRSTAT0 & (1<<0)) == 1)
		{
			flag = 1;
			break;
		}
		
		if ( (i & 0x00fffff) == 0 )
			printf("%d ", 3 - (i>>20));
			
		i++;
	}
		
	printf("wait time over! \n");
	
	if (flag == 0)	
		autorun();
	
	// auto load wav file from 0x100000
	loadwav();
		
	// UTXH0 -> data
	while (1)
	{
		puts("akaedu $ ");
		gets(buf);
		
		printf("your input: <%s>\n", buf);
			
		argc = shell_parse(buf, argv);
		
		for (i = 0; i < argc; i++)
			printf("%d: <%s>\n", i, argv[i]);	
						
		command_do(argc, argv);		
	}
	
	while(1);
	
	return 0;
}