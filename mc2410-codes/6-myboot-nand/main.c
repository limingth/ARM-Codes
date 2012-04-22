#include "uart.h"
#include "stdio.h"
#include "shell.h"
#include "lib.h"
#include "command.h"
#include "nand.h"

#define MPLLCON	(*(volatile unsigned int *)(0x4C000004))
#define UPLLCON	(*(volatile unsigned int *)(0x4C000008))
#define CLKCON	(*(volatile unsigned int *)(0x4C00000C))
#define CLKDIVN	(*(volatile unsigned int *)(0x4C000014))

#define WTCON	(*(volatile unsigned int *)(0x53000000))

#define BWSCON	(*(volatile unsigned int *)(0x48000000))

void sys_init()
{
	// watch dog disable
	WTCON = 0x0;
	
	// DW6 [25:24] Determine data bus width for bank 6.
	// 00 = 8-bit      01 = 16-bit,       
	// 10 = 32-bit       11 = reserved
	BWSCON = 1<<25;
	
	// 1:2:4	
	// PCLK:HCLK:FCLK = 50M:100M:200M
	CLKDIVN = 0x1<<0 | 0x1<<1;

	MPLLCON = 0x0005C040;	
}

int mymain(void)
{
	char buf[200];
	int argc = 0;
	char * argv[8];
	int i;
	
	// clock init, watch dog disable
	sys_init();
		
	// UART init
	uart_init();

	/* nand_flash init */
	nand_init();
	nand_reset();
	
	printf("\n\nWelcome to my boot v1.0 \n");
	printf("Date is %s\n", __DATE__);
	printf("Version is 0x%x\n", 0x87654321);
	
	while(1)	
	{	
		printf("Jobs $ ");
		gets(buf);	
		printf("user input: <%s>\n", buf);
		
		// shell parse buf -> argc, argv[]
		shell_parse(buf, &argc, argv);
		
		for (i = 0; i < argc; i++)		
			printf("%x: <%s>\n", i, argv[i]);
		
		// command: help
		if (strcmp(argv[0], "help") == 0)
			help(argc, argv);
		
		if (strcmp(argv[0], "md") == 0)
			md(argc, argv);	
		
		if (strcmp(argv[0], "mw") == 0)
			mw(argc, argv);	
		
		if (strcmp(argv[0], "load") == 0)
			load(argc, argv);	
			
		if (strcmp(argv[0], "go") == 0)
			go(argc, argv);	
			
		if (strcmp(argv[0], "loadx") == 0)
			loadx(argc, argv);
		
		if (strcmp(argv[0], "flashw") == 0)
			flashw(argc, argv);
			
		if (strcmp(argv[0], "flashl") == 0)
			flashl(argc, argv);					
	}
	
	return 0;
}