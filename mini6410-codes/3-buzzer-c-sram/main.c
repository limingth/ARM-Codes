
#define GPKCON0 (*(volatile unsigned int *)0x7F008800)
#define GPKDAT (*(volatile unsigned int *)0x7F008808)

#define GPFCON (*(volatile unsigned int *)0x7F0080A0)
#define GPFDAT (*(volatile unsigned int *)0x7F0080A4)


void delay(void)
{
	int i;
	for( i = 0x500000; i >= 0; i-- )
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

/* buzzer PWM0 = GPF14 */
void buzzer_on()
{	
	GPFDAT |= 1<<14;
	
	return;
}

void buzzer_off()
{
	GPFDAT &= ~(1<<14);
	
	return;
}

void mymain(void)
{
	// set GPKCON0: output	(led1-led4: use GPK4-GPK7)
	GPKCON0 = 0x11110000;	
	
	// set all led off (write 1 to be off)
	GPKDAT = 0x000000f0;
	
	// set GPF14 as output
	GPFCON |= 1<<28;
	GPFCON &= ~(1<<29);
	
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
		buzzer_on();
		
		led_on(0);
		delay();
		led_off(0);
		
		buzzer_off();
		
		led_on(1);
		delay();
		led_off(1);
		
		buzzer_on();
		
		led_on(2);
		delay();
		led_off(2);
		
		buzzer_off();
		
		led_on(3);
		delay();
		led_off(4);
	}	
}