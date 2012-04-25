
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
	for( i = 0x50000; i >= 0; i-- )
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
	UBRDIV0 = 0x2D;	
	
	c = 'a';
	
	while(1)
	{
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

#if 0		
		UTXH0 = 'a';
		delay();
		UTXH0 = 'b';
		delay();
#endif
	
		//c = uart0_getchar();
		/*
		 UCON0 is 0xC05	
		 ULCON0 is 0x3	
		 UBRDIV0 is 0x15F	
		 GPACON is 0x2222
		 buad = 14400
		 clk = 81100800
		 
		 UBRDIV0 is 0x2D
		 buad = 115200	
		 */
	
		uart0_putchar(c);
		my_printf(" %c is %d(0x%x)	\n", c, c, c);
		my_printf(" UCON0 is 0x%x	\n", UCON0);
		my_printf(" ULCON0 is 0x%x	\n", ULCON0);
		my_printf(" UBRDIV0 is 0x%x	\n", UBRDIV0);
		my_printf(" GPACON is 0x%x	\n", GPACON);
		
		c++;
		if (c > 'z')
			c = 'a';
			
		delay();		
	}	
}