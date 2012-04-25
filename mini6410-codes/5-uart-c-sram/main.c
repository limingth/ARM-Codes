
#define GPKCON0 (*(volatile unsigned int *)0x7F008800)
#define GPKDAT (*(volatile unsigned int *)0x7F008808)

#define GPFCON (*(volatile unsigned int *)0x7F0080A0)
#define GPFDAT (*(volatile unsigned int *)0x7F0080A4)

#define GPNCON (*(volatile unsigned int *)0x7F008830)
#define GPNDAT (*(volatile unsigned int *)0x7F008834)

#define ULCON0 (*(volatile unsigned int *)0x7F005000)
#define UCON0  (*(volatile unsigned int *)0x7F005004)
#define UBRDIV0 (*(volatile unsigned int *)0x7F005028)

#define UTXH0 (*(volatile unsigned int *)0x7F005020)
#define URXH0 (*(volatile unsigned int *)0x7F005024)
#define UTRSTAT0 (*(volatile unsigned int *)0x7F005010)

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

char hex(int c)
{
	if (c >= 0 && c<=9)
		return c + '0';

	return c - 10 + 'a';
}

void mymain(void)
{
	char c;
	unsigned char ubrdiv0 = 0;
	//char hex[17] = "0123456789abcdef";
	
	// set GPKCON0: output	(led1-led4: use GPK4-GPK7)
	GPKCON0 = 0x11110000;	
	
	// set all led off (write 1 to be off)
	GPKDAT = 0x000000f0;
	
	// set GPF14 as output
	GPFCON |= 1<<28;
	GPFCON &= ~(1<<29);
	
	// set GPN0 as input
	GPNCON = 0;
	
	// uart0 init
#if 1
	// set GPA0-3 function as UART
#define GPACON (*(volatile unsigned int *)0x7F008000)		
	GPACON = 1<<1 | 1<<5 | 1<<9 | 1<<13;
	
	// 8 N 1
	ULCON0 = 1<<0 | 1<<1;
	
	// enable transmit and receive 
	UCON0 = 1<<0 | 1<<2 | 1<<10 | 1<<11; 	// ? ÓÐÎÊÌâ
	//UCON0 = 0xE45; 
#endif
	/* after boot, UBRDIV0 is 0x22 -> EXT_UCLK1=64512000 */
	//UBRDIV0 = 209; // 209;
	UBRDIV0 = ubrdiv0;	// 115200 -> 0x2D 

	while(1)
	{
		UBRDIV0 = ubrdiv0;
		led_on(0);
		
		uart0_putchar('h');		
		uart0_putchar('e');		
		uart0_putchar('l');		
		uart0_putchar('l');		
		uart0_putchar('o');		
		uart0_putchar('\r');
		uart0_putchar('\n');
		
		for (c = 0; c < 16; c++)
			uart0_putchar(hex(c));

		uart0_putchar('\r');
		uart0_putchar('\n');
		uart0_putchar('0');
		uart0_putchar('x');
		uart0_putchar( hex(ubrdiv0>>4));
		uart0_putchar( hex(ubrdiv0&0x0f));				
		uart0_putchar('\r');
		uart0_putchar('\n');
		delay();
		led_off(0);		
		
		if ( !(GPNDAT & (1<<0)) )
		{
			uart0_putchar('0');
			uart0_putchar('x');
			uart0_putchar( hex(ubrdiv0>>4));
			uart0_putchar( hex(ubrdiv0&0x0f));
			while(1);
		}		
		
		ubrdiv0 += 1;
		continue;	

#if 0
		if ( !(GPNDAT & (1<<0)) )
		{
			buzzer_on();
			led_on(0);
			UBRDIV0 += 1;	
		}
		else
		{
			buzzer_off();
			led_off(0);
		}			
	
		//c = uart0_getchar();
		uart0_putchar(c);
		c++;
		if (c > 'z')
			c = 'a';
		delay();	
#endif
	}	
}