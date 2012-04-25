#define LED	(*(char *)0x20800000)

#define GPBCON	(*(volatile unsigned int *)0x56000010)
#define GPBDAT	(*(volatile unsigned int *)0x56000014)

#define WTCON	(*(volatile unsigned int *)0x53000000)

int mymain(void)
{
	int i;
	
	WTCON = 0;
	
	LED = 0x0;
	
	for (i = 0; i < 500000; i++)
		;

	LED = 0xFF;
	
	GPBCON = 1;
	
	// beep off
	GPBDAT = 0;
#if 0	
	for (i = 0; i < 100000; i++)
		;
		
	// beep on
	GPBDAT = 1;

	for (i = 0; i < 100000; i++)
		;

	GPBDAT = 0;
	GPBDAT = 1;
	GPBDAT = 0;
#endif	
	while(1)
	{
		LED = 0xFF;
		for (i = 0; i < 100000; i++)
			;
			
		LED = 0x00;
		for (i = 0; i < 100000; i++)
			;
	}
	
	while(1);
	
	return 0;
}