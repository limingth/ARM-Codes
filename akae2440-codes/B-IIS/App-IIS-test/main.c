
#define UTRSTAT0 (*(volatile unsigned int *)0x50000010)    
#define UTXH0	(*(volatile unsigned int *)0x50000020)    

#define IISFIFO	(*(volatile unsigned int *)0x55000010)
#define IISCON	(*(volatile unsigned int *)0x55000000)
#define IISMOD	(*(volatile unsigned int *)0x55000004)
#define IISFCON	(*(volatile unsigned int *)0x5500000C)
#define IISPSR	(*(volatile unsigned int *)0x55000008)

#define GPECON	(*(volatile unsigned int *)0x56000040)

#define GPBCON (*(volatile unsigned int *)0x56000010)
#define GPBDAT (*(volatile unsigned int *)0x56000014)


void uart_putchar(char c)
{
	// polling status
	while ((UTRSTAT0 & (1<<2)) == 0) 
		;
	
	UTXH0 = c;	
}

void delay(void)
{
	int i = 0;
	
	for (i = 0; i < 10000; i++)
		;
}

void putchar_hex(char c)
{
	char * hex = "0123456789ABCDEF";	// good
		
	uart_putchar(hex[(c & 0xf0)>>4]);
	uart_putchar(hex[(c & 0x0f)>>0]);
	uart_putchar(' ');
}

void putint_hex(int a)
{
	putchar_hex( (a>>24) & 0xFF );
	putchar_hex( (a>>16) & 0xFF );
	putchar_hex( (a>>8) & 0xFF );
	putchar_hex( (a>>0) & 0xFF );
}

void L3_init()
{
	// GPB2,3,4 = output
	GPBCON |= 1<<4 | 1<<6 | 1<<8;
	GPBCON &= ~(1<<5 | 1<<7 | 1<<9);
}

/* L3 interface */
// GPB2 - L3MODE
// GPB3 - L3DATA
// GPB4 - L3CLOCK
#define L3MODE(v)	(v==0)?  (GPBDAT &= ~(1<<2)) : (GPBDAT |= 1<<2)
#define L3DATA(v)	(v==0)?  (GPBDAT &= ~(1<<3)) : (GPBDAT |= 1<<3) 
#define L3CLOCK(v)	(v==0)?  (GPBDAT &= ~(1<<4)) : (GPBDAT |= 1<<4) 	

/* 200Mhz cpu clock => 1 instruction = 5ns */
#define DELAY(t)	do { \
				int i; \
				for (i = 0; i < t; i++); \
			} while(0)	

void L3_data_shift(int data)
{
	int i;
	
	for (i = 0; i < 8; i++)
	{
		int bit;
		
		bit = (data & (1<<i))? 1: 0;
		
		// write data 1 bit 
		// Tsu(L3)DA - L3DATA set-up time > 190ns	
		L3DATA(bit);
		
		// Tclk(L3)L - L3CLOCK low time > 250ns
		// CLOCK 0 -> 1 : rising edge
		L3CLOCK(0);
		DELAY(250);	
		L3CLOCK(1);
		DELAY(250);	
	}	
}

void L3_address_mode(int data)
{	
	// Th(L3)A - L3MODE hold time > 190ns
	L3CLOCK(1);
	L3MODE(1);
	DELAY(190);	
	
	// Tsu(L3)A - L3MODE set-up time > 190ns
	L3MODE(0);	// address mode
	DELAY(190);
	
	L3_data_shift(data);
	
	// Th(L3)A - L3MODE hold time > 190ns
	L3CLOCK(1);
	DELAY(190);
	L3MODE(1);
	DELAY(190);
}

void L3_data_transfer_mode(int data)
{
	L3MODE(1);	// data transfer mode
		
	// Tstp(L3)A - L3MODE halt time > 190ns
	L3MODE(0);	// data transfer mode
	DELAY(190);
	
	// Tsu(L3)A - L3MODE set-up time > 190ns
	L3CLOCK(1);
	L3MODE(1);
	DELAY(190);
	
	L3_data_shift(data);
	
	// Th(L3)A - L3MODE hold time > 190ns
	L3CLOCK(1);
	DELAY(190);
	L3MODE(0);
	DELAY(190);
	
	L3MODE(1);	// data transfer mode
}

void UDA1341_init(void)
{
	/* UDA1341 init */
	L3_address_mode(0x14 + 2);	// 000101xx + 10 (STATUS CONTROL)
	L3_data_transfer_mode(0x50);	// 0,1,01,000,0 : 1-reset, 384fs
	delay();
	
	L3_address_mode(0x14 + 0);	// 000101xx + 00 (DATA0 CONTROL)
	L3_data_transfer_mode(0x02);	// -1 db (volume)
	delay();
}


void IIS_set_transmit_mode(void)
{
	// IIS interface [0] 	0 = Disable (stop)
	// 			1 = Enable  (start)
	IISCON |= 1<<0;
	
	// IIS prescaler [1] 	0 = Disable
	// 			1 = Enable
	IISCON |= 1<<1;
	
	// Transmit/receive mode select : Transmit and receive
	// [7:6] 00 = No transfer          01 = Receive mode
	// 	 10 = Transmit mode    11 = Transmit and receive mod
	IISMOD |= 1<<6 | 1<<7;
	
	// Serial data bit per channel : 16-bit
	// [3] 0 = 8-bit         1 = 16-bit
	IISMOD |= 1<<3;
	
	// Master clock frequency select : 384fs
	// [2] 0 = 256fs        1 = 384fs
	//	(fs: sampling frequency)
	IISMOD |= 1<<2;
	
	// Serial bit clock frequency select : 32fs
	// [1:0] 00 = 16fs        01 = 32fs
	//	10 = 48fs        11 = N/A
	IISMOD |= 1<<0;
	
	//  Prescaler B makes the master clock that is used the external
        //   block and division factor is N+1
	// N + 1 = 50 * 1000000 / (384 * 44100) = 2.959
	// N = 2;
	// N + 1 = 50 * 1000000 / (384 * 22050) = 5.9
	// N = 5	
	IISPSR = 5<<0 | 5<<5;
		
	// Transmit FIFO [13] 0 = Disable     1 = Enable
	IISFCON |= 1<<13;
}

void IIS_init(void)
{
	// GPE0,1,2,3,4
	// [9:8]	10 = I2SDO 
	// [7:6]	10 = I2SDI   
	// [5:4]	10 = CDCLK   
	// [3:2]	10 = I2SSCLK 
	// [1:0]	10 = I2SLRCK  
	GPECON |= 1<<1 | 1<<3 | 1<<5 | 1<<7 | 1<<9;
	
	// IIS init
	// IIS interface [0] 	0 = Disable (stop)
	//			1 = Enable  (start)
	IISCON |= 1<<0;
	
	// Transmit/receive mode select
	// [7:6] 00 = No transfer      01 = Receive mode
	//	 10 = Transmit mode    11 = Transmit and receive mode	
	IISMOD |= 1<<7;
	
	// Transmit FIFO [13] 0 = Disable     1 = Enable
	IISFCON |= 1<<13;

	// Master clock frequency select
	//	[2] 0 = 256fs        1 = 384fs
	//	(fs: sampling frequency)
	IISMOD |= 1<<2;	
	
	// IIS prescaler [1] 0 = Disable	1 = Enable
	IISCON |= 1<<1;	
	
	// PCLK / (N+1) = master clock
	// master clock = 384 * 22Khz
	// PCLK = 50M = 50000 Khz
	// N+1 = 50000Khz/ (384*22Khz) = 5.91 = 6
	IISPSR = 5<<5 | 5<<0;		// 22Khz
	// IISPSR = 2<<5 | 2<<0;	// 44Khz
	
	// Serial data bit per channel
	//	[3] 	0 = 8-bit         1 = 16-bit
	IISMOD |= 1<<3;
	
	// Serial bit clock frequency select
	//	[1:0] 	00 = 16fs        01 = 32fs
	//		10 = 48fs        11 = N/A
	IISMOD |= 1<<0;	
}

void IIS_trans_data(short * pdata, int size)
{
	while (size > 0)
	{
		while((IISCON & (1<<7)) == (1<<7))
			;
		
		IISFIFO = *pdata;	
		
		pdata++;
		
		size -= 2;
	}
	
	return;
}

void IIS_playwav(int wav_addr)
{
	int size = 0x100000;
	short * p_wavdata;
	char * p_datasize;
	int offset;
	char c0, c1, c2, c3;

	offset = *(int *)(wav_addr + 0x10);
	if (offset == 0x10)
	{	
		p_datasize = (char *)(wav_addr + 0x24+4);
		p_wavdata = (short *)(wav_addr+0x24+4+4);
	}
	else
	{
		p_datasize = (char *)(wav_addr + 0x26+4);
		p_wavdata = (short *)(wav_addr+0x26+4+4);
	}
	
	c0 = *(p_datasize+0);
	c1 = *(p_datasize+1);
	c2 = *(p_datasize+2);
	c3 = *(p_datasize+3);
	
	size = (c0<<0) | (c1<<8) | (c2<<16) | (c3<<24);
	
//	putint_hex(size);
	
	IIS_trans_data(p_wavdata, size);		
}

int mymain(void)
{	

	IIS_init();
	
	IIS_playwav(0x32000000);
	
//	IIS_playwav(0x33000000);

	uart_putchar('$');
	
	while (1);


//	L3_init();
//	UDA1341_init();
#if 0
	IIS_init();
	IIS_set_transmit_mode();
	L3_init();
	UDA1341_init();
#endif	

	return 0;
}