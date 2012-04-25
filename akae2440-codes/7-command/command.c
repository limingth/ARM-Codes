#include "stdio.h"

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

int command_do(int argc, char * argv[])
{
	if (strcmp(argv[0], "help") == 0)
		help(argc, argv);
		
	if (strcmp(argv[0], "md") == 0)
		md(argc, argv);

	if (strcmp(argv[0], "mw") == 0)
		mw(argc, argv);

	return 0;
}