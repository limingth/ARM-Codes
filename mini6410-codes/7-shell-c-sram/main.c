
#define GPKCON0 (*(volatile unsigned int *)0x7F008800)
#define GPKDAT (*(volatile unsigned int *)0x7F008808)

#define GPFCON (*(volatile unsigned int *)0x7F0080A0)
#define GPFDAT (*(volatile unsigned int *)0x7F0080A4)

#define GPNCON (*(volatile unsigned int *)0x7F008830)
#define GPNDAT (*(volatile unsigned int *)0x7F008834)

#define UTXH0 (*(volatile unsigned int *)0x7F005020)
#define URXH0 (*(volatile unsigned int *)0x7F005024)
#define UTRSTAT0 (*(volatile unsigned int *)0x7F005010)

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

void uart0_putchar(char c)
{
		while( !(UTRSTAT0 & (1<<2)) )
			;
			
		UTXH0= c;
}

char uart0_getchar(void)
{
	char c;
	
	while ( !(UTRSTAT0 & (1<<0)) )
		;
		
	c = URXH0;
	
	return c;
}

extern int my_printf(const char *format, ...);

void mymain(void)
{
	char c;
	
	// set GPKCON0: output	(led1-led4: use GPK4-GPK7)
	GPKCON0 = 0x11110000;	
	
	// set all led off (write 1 to be off)
	GPKDAT = 0x000000f0;
	
	// set GPF14 as output
	GPFCON |= 1<<28;
	GPFCON &= ~(1<<29);
	
	// set GPN0 as input
	GPNCON = 0;
	
	buzzer_on();
	delay();
	buzzer_off();
	my_shell();
	
	c = 'a';
	
	while(1)
	{
		if ( !(GPNDAT & (1<<0)) )
		{
			buzzer_on();
			led_on(0);
		}
		else
		{
			buzzer_off();
			led_off(0);
		}			

#if 0		
		UTXH0 = 'a';
		delay();
		UTXH0 = 'b';
		delay();
#endif
		
		//c = uart0_getchar();
		uart0_putchar(c);
		my_printf(" %c is %d(0x%x)	\n", c, c, c);
		
		c++;
		if (c > 'z')
			c = 'a';	
	}	
}