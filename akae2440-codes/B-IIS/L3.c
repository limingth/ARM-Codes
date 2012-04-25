/* L3 interface */
// GPB2 - L3MODE
// GPB3 - L3DATA
// GPB4 - L3CLOCK
#define	L3MODE_1	GPBDAT |= 1<<2
#define	L3MODE_0	GPBDAT &= ~(1<<2)

#define	L3DATA_1	GPBDAT |= 1<<3
#define	L3DATA_0	GPBDAT &= ~(1<<3)

#define	L3CLOCK_1	GPBDAT |= 1<<4
#define	L3CLOCK_0	GPBDAT &= ~(1<<4)

#define GPBCON (*(volatile unsigned int *)0x56000010)
#define GPBDAT (*(volatile unsigned int *)0x56000014)

void delay_ns(int t)
{
	int i;
	
	for (i = 0; i < t; i++)
		;	
}

void L3_init()
{
	// GPB2,3,4 = output
	GPBCON |= 1<<4 | 1<<6 | 1<<8;
	GPBCON &= ~(1<<5 | 1<<7 | 1<<9);
}

void L3_address(char addr)
{
	int i = 0;
	
	// Mode falling edge
	L3CLOCK_1;	// clock high
	L3MODE_1;
	delay_ns(190);	// mode hold time = 190
	L3MODE_0;
	
	delay_ns(190);	// address mode set-up 190ns
	
	for (i = 0; i < 8; i++)
	{
		if (addr & (1<<i))
			L3DATA_1;	// bit = 1
		else
			L3DATA_0;	// bit = 0		
		
		L3CLOCK_0;	// clock low 
		delay_ns(250);	// clock LOW time = 250
		L3CLOCK_1;	// clock high, rising edge
		delay_ns(250);	// clock HIGH time = 250
	}
	
	delay_ns(250);
	L3MODE_1;		// idle 
	delay_ns(250);	
}

void L3_write_data(char data)
{
	int i = 0;
	
	// Mode falling edge
	L3CLOCK_1;	// clock high
	L3MODE_0;
	delay_ns(190);	// mode hold time = 190
	L3MODE_1;
	
	delay_ns(190);	// address mode set-up 190ns
	for (i = 0; i < 8; i++)
	{
		if (data & (1<<i))
			L3DATA_1;	// bit = 1
		else
			L3DATA_0;	// bit = 0		
		
		L3CLOCK_0;	// clock low 
		delay_ns(250);	// clock LOW time = 250
		L3CLOCK_1;	// clock high, rising edge
		delay_ns(250);	// clock HIGH time = 250
	}
	
	delay_ns(250);
	L3MODE_0;
	delay_ns(250);	
	L3MODE_1;		// idle 
}