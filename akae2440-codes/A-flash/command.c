#include "stdio.h"
#include "xmodem.h"
#include "nand.h"

int help(int argc, char * argv[])
{
	printf("help usage: .....\n");
	return 0;
}

int atoi(char * buf)
{
	int value = 0;	
	int base = 10;
	int i = 0;
	
	if (buf[0] == '0' && buf[1] == 'x')
	{
		base = 16;
		i = 2;
	}
	
	// 123 = (1 * 10 + 2) * 10 + 3
	// 0x1F = 1 * 16 + F(15)	
	while (buf[i])
	{
		int tmp;
		
		if (buf[i] <= '9' && buf[i] >= '0') 
			tmp = buf[i] - '0';
		else
			tmp = buf[i] - 'a' + 10;
					
		value = value * base + tmp;
		
		i++;
	}
	
	return value;
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

int strcmp(const char * s1, const char * s2)
{
	while (*s1 == *s2)
	{
		if (*s1 == '\0')
			return 0;	
		s1++;
		s2++;				
	}
	
	return *s1 - *s2;
}

//#define DOWN_BIN_ADDR	(0xC00)
#define DOWN_BIN_ADDR	(0x31000000)

int loadb(int argc, char * argv[])
{
	int i = 0;
	int size = 0;
	char * p = (char *)DOWN_BIN_ADDR;
		
	if (argc >= 2)
		size = atoi(argv[1]);
	
	printf("load bin file to address 0x%x\n", DOWN_BIN_ADDR);
	
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
	int go_addr = DOWN_BIN_ADDR;	
	
	if (argc >= 2)
		go_addr = atoi(argv[1]);
	
	printf("go to address 0x%x\n", go_addr);
	
	fp = (void (*)(void))go_addr;
	
	fp();
	
	return 0;
}

void nandread(int argc, char *argv[])
{	
	// nandread 0x0 (flash)		0x30000000 (sdram)	512 (size)
	int flash_addr;
	char * buf;
	int size;
	
	if (argc <= 3)
	{
		printf("nandread flash_addr sdram_addr	size\n");
		printf("nandread 0x100000 0x30000000 512 \n");
		return;
	}
		
	flash_addr = atoi(argv[1]);
	buf = (char *)atoi(argv[2]);
	size = atoi(argv[3]);
	
	nand_init();
	nand_read(flash_addr, buf, size);

	printf("nandread finished! \n");
}

int command_do(int argc, char * argv[])
{
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

	if (strcmp(argv[0], "nandread") == 0)
		nandread(argc, argv);

	return 0;
}