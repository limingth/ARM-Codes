
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

void delay_ns(int t)
{
	int i;
	
	for (i = 0; i < t; i++)
		;	
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

void IIS_trans_data_dma(short * pdata, int size)
{
	//init IIS = DMA mode
	//Transmit FIFO access mode select [15] 0 = Normal 1 = DMA
	IISFCON |= 1<<15;
	//init dma mem->IISFIFO
	//Transmit DMA service request [5] 0 = Disable 1 = Enable
	IISCON |= 1<<5;
	
#define DISRC2 		(*(volatile unsigned int *)0x4B000080)
#define DISRCC2 	(*(volatile unsigned int *)0x4B000084)
#define DIDST2 		(*(volatile unsigned int *)0x4B000088)
#define DIDSTC2 	(*(volatile unsigned int *)0x4B00008C)
#define DCON2 		(*(volatile unsigned int *)0x4B000090)
#define DMASKTRIG2 	(*(volatile unsigned int *)0x4B0000A0)
	//start dma
	DISRC2 = (int)pdata;
	DISRCC2 = 0x0;
	DIDST2 = (int)0x55000010;
	DIDSTC2 = 1<<1 | 1<<0;
	DCON2 = (unsigned int)size/2;
	DCON2 |= 1<<23 | 1<<20;
	DMASKTRIG2 = 1<<1;
	
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
	
	putint_hex(size);
	
//	IIS_trans_data(p_wavdata, size);		
	IIS_trans_data_dma(p_wavdata, size);
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

int mymain(void)
{	
	// L3 bus
	L3_init();
	L3_address(0x14 + 0x2);		// 0b000101,10 Status 
	L3_write_data(1<<6 | 1<<4);	// 1<<6 reset, 1<<4 384fs 
	delay();
	
	L3_address(0x14 + 0x0);		// 0b000101,00 DATA0
//	L3_write_data(0x0f);		// -15dB volume
	L3_write_data(0x0);		// 0dB volume
//	L3_write_data(0x3d);		// -60dB volume	
	delay();
	
	// IIS bus
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