#include "stdio.h"
#include "lib.h"
#include "xmodem.h"

int help(int argc, char * argv[])
{
	printf("help usage: \n");
	printf("md - memory dispaly\n");
	printf("mw - memory write\n");
	printf("loadb - load .bin file from uart\n");
	printf("go - go to address\n");
	printf("loadx - load .bin file using xmodem\n");

	return 0;
}

int md(int argc, char * argv[])
{	
	int * p = (int *)0;
	int i, j;
	
	if (argc >= 2)
		p = (int *)atoi(argv[1]);
		
	for (j = 0; j < 16; j++)
	{	
		printf("%x: ", (int)p);
		for (i = 0; i < 4; i++)
			printf("%x ", *p++);	
		printf("\n");
	}
		
	return 0;
}

// mw 0x0 0x1122
int mw(int argc, char * argv[])
{	
	int * p = (int *)0;
	int v = 0;
	
	if (argc >= 2)
		p = (int *)atoi(argv[1]);
		
	if (argc >= 3)
		v = atoi(argv[2]);
		
	*p = v;
	
	return 0;
}


#define DOWN_BIN_ADDR	(0xD0022000)	// 0xD0020000 + 8K as .bin file load addr

int loadb(int argc, char * argv[])
{
	int i = 0;
	int size = 0;
	char * p = (char *)DOWN_BIN_ADDR;
	
	printf("load bin file to address 0x%x\n", DOWN_BIN_ADDR);
		
	if (argc >= 2)
		size = atoi(argv[1]);
	
	for (i = 0; i < size; i++)
		*p++ = getchar();
	
	printf("load finished! \n");
	
	return 0;
}

void loadx(int argc, char *argv[])
{	
	char * p = (char *)DOWN_BIN_ADDR;
	
	if (argc >= 2)
		p = (char *)atoi(argv[1]);
	
	printf("load bin file by xmodem to address 0x%x\n", (int)p);		
			
	xmodem_recv(p);
	
	printf("loadx finished! \n");
}

int go(int argc, char * argv[])
{
	void (*fp)(void);
	
	printf("go to address 0x%x\n", DOWN_BIN_ADDR);

	fp = (void (*)(void))DOWN_BIN_ADDR;
	
	fp();
	
	return 0;
}

int command_do(int argc, char * argv[])
{
	printf("command do <%s> \n", argv[0]);

	if (strcmp(argv[0], "help") == 0)
		help(argc, argv);
		
	if (strcmp(argv[0], "md") == 0)
		md(argc, argv);

	if (strcmp(argv[0], "mw") == 0)
		mw(argc, argv);
	
	if (strcmp(argv[0], "loadb") == 0)
		loadb(argc, argv);

	if (strcmp(argv[0], "go") == 0)
		go(argc, argv);
		
	if (strcmp(argv[0], "loadx") == 0)
		loadx(argc, argv);

	return 0;
}
