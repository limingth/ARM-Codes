
#define GPKCON0 (*(volatile unsigned int *)0x7f008800)
#define GPKDAT (*(volatile unsigned int *)0x7f008808)

void delay(void)
{
	int i;
	for( i = 0x100000; i >= 0; i-- )
		;
	
	return;
}


static int led_pos[4] = { 1<<4, 1<<5, 1<<6, 1<<7 };

void led_off(int i)
{	
	GPKDAT |= led_pos[i];
	
	return;
}

void led_on(int i)
{
	GPKDAT &= ~(led_pos[i]);
	
	return;
}

void mymain(void)
{
	// set GPKCON0: output	(led1-led4: use GPK4-GPK7)
	GPKCON0 = 0x11110000;	
	
	// set all led off (write 1 to be off)
	GPKDAT = 0x000000f0;
	
#if 0	
	while(1)
	{
		// set GPKDAT: let led on
		GPKDAT = 0x000000f0;	
		delay();
		GPKDAT = 0x0;
		delay();
	}
#endif

	while(1)
	{
		led_on(0);
		delay();
		led_off(0);
		
		led_on(1);
		delay();
		led_off(1);
		
		led_on(2);
		delay();
		led_off(2);
		
		led_on(3);
		delay();
		led_off(4);
	}	
}