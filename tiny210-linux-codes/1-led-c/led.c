#define GPJ2CON		(*(volatile unsigned int *)0xe0200280)
#define GPJ2DAT		(*(volatile unsigned int *)0xe0200284)

#define GPH2CON		(*(volatile unsigned int *)0xE0200C40)
#define GPH2DAT		(*(volatile unsigned int *)0xE0200C44)

void delay(void)
{
	int i = 0;

	for (i = 0; i < 100000; i++)
		;
}

int mymain(void)
{
	// set GPJ2_0 as output
	GPJ2CON = 0x1111;

	// set GPJ2_0 output 0x0 (low level) = led1 on
	GPJ2DAT = 0x0;
	
	// set GPH2_0 as input
	GPH2CON = 0x0;

#if 0
	while (1)
	{		
		GPJ2DAT = 0x5;	
		delay();

		GPJ2DAT = 0xa;	
		delay();
	}
#endif

	while (1)
	{		
		if ((GPH2DAT & 0x1) == 0)	
			GPJ2DAT = 0x0;	
		else
			GPJ2DAT = 0x1;	
	}
	
	while(1);

	return 0;
}
