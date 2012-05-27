
// GPIO
#define GPICON  	(*(volatile unsigned int *)0xE0200220)	//IIS Signals

// IIS
#define IISCON  	(*(volatile unsigned int *)0xEEE30000)	//IIS Control
#define IISMOD  	(*(volatile unsigned int *)0xEEE30004)	//IIS Mode
#define IISFIC  	(*(volatile unsigned int *)0xEEE30008)	//IIS FIFO Control
#define IISPSR  	(*(volatile unsigned int *)0xEEE3000C)	//IIS Prescaler
#define IISTXD		(*(volatile unsigned int *)0xEEE30010)	//IIS TXD DATA
#define IISRXD 		(*(volatile unsigned int *)0xEEE30014)	//IIS RXD DATA
#define IISFICS  	(*(volatile unsigned int *)0xEEE30018)	//IIS FIFO Control

void IIS_init(void)
{
	int N;
	// IIS signals - Xi2s0SCLK  Xi2s0CDCLK  Xi2s0LRCK  Xi2s0SDI  Xi2s0SDO
	GPICON = 0x22222222;

	// IIS clock init
	// step 1: EPLL output 67.7Mhz (see p361 of s5pv210.pdf)
	// EPLL_CON0/ EPLL_CON1, R/W, Address = 0xE010_0110/0xE010_0114)
	// FOUT = (MDIV+K/65536) X FIN / (PDIV X 2SDIV)
	// Fout = (0x43+0.7)*24M / (3*2^3) = 80*24M/24 = 67.7Mhz
#define EPLL_CON0  	(*(volatile unsigned int *)0xe0100110)
#define EPLL_CON1  	(*(volatile unsigned int *)0xe0100114)
	EPLL_CON0 = 0xa8430303;		// MPLL_FOUT = 67.7Mhz
	EPLL_CON1 = 0xbcee;		// from linux kernel setting
	
	// step 2: MUX_EPLL = 1 -> SCLK_EPLL = 67.7Mhz (see p361 of s5pv210.pdf)
	// CLK_SRC0, R/W, Address = 0xE010_0200
	// 0xe0100200: 10001111
	// EPLL_SEL  [8]  Control MUXEPLL (0:FINPLL, 1:FOUTEPLL) 
#define CLK_SRC0  	(*(volatile unsigned int *)0xE0100200)		
	CLK_SRC0 = 0x10001111;
	
	// step 3: Mux_I2S  AUDIO subsystem clock selection (see P1868 P1875 of s5pv210.pdf)
#define CLK_CON  	(*(volatile unsigned int *)0xEEE10000)	
	CLK_CON = 0x1;		// 1 = FOUT_EPLL 		MUXI2S_A 00 = Main CLK 	
	
#if 0		// It seems no use for IIS, I am still thinking why
	// CLK_SRC6, R/W, Address = 0xE010_0218) 	AUDIO0_SEL  [3:0] 	0111: SCLKEPLL, 
	// Control MUXAUDIO0, which is the source clock of AUDIO0 
	// (0000: XXTI, 0001: PCMCDCLK0, 0010: SCLK_HDMI27M, 
	//	0011: SCLK_USBPHY0, 0100: SCLK_USBPHY1, 0101: SCLK_HDMIPHY, 
	//	0110: SCLKMPLL, 0111: SCLKEPLL, 100: SCLKVPLL, OTHERS: reserved) 
#define CLK_SRC6  	(*(volatile unsigned int *)0xE0100218)	
//	CLK_SRC6 = 0x7;	// 0111: SCLK_EPLL	from linux setting
#endif
		
	// IIS SFRs		
	// step 4:	Divider of IIS (67.7 -> 11.289Mhz)
	// N + 1 = (67.7Mhz) / (256 * 44.1Khz) = 5.99
	// IISCDCLK  11.289Mhz = 44.1K * 256fs 
	// IISSCLK    1.4112Mhz = 44.1K * 32fs
	// IISLRCLK   44.1Khz
	N = 5;
	IISPSR = 1<<15 | N<<8;
	
	// Now we set IIS SFR
	// IIS interface active (start operation). 	1 = Active 	
	IISCON |= 1<<0 | (unsigned)1<<31;
	
	// [9:8] 10 = Transmit and receive simultaneous mode 	
	// 1 = Using I2SCLK 	(use EPLL)
	IISMOD = 1<<9 | 0<<8 | 1<<10;
}

// IIC GPIO
#define GPD1CON    	(*(volatile unsigned *)0xE02000C0) //Port D1 control
#define GPD1DAT    	(*(volatile unsigned *)0xE02000C4) //Port D1 data
#define GPD1PUD    	(*(volatile unsigned *)0xE02000C8) //Pull-up control D

// IIC SFR
#define I2CCON0  	(*(volatile unsigned *)0xE1800000) //IIC control
#define I2CSTAT0 	(*(volatile unsigned *)0xE1800004) //IIC status
#define I2CADD0  	(*(volatile unsigned *)0xE1800008) //IIC address
#define I2CDS0   	(*(volatile unsigned *)0xE180000C) //IIC data shift
#define I2CLC0   	(*(volatile unsigned *)0xE1800010) //IIC multi-master line control 

// for portability of IIC driver
#define IICCON	I2CCON0
#define IICSTAT	I2CSTAT0
#define IICADD	I2CADD0
#define IICDS	I2CDS0

void IIC_write(int slave_addr, int addr, int data)
{
	IICDS = slave_addr;
	IICSTAT = 0xf0;
	while ((IICCON & 0x10) == 0);	// INT
	while ((IICSTAT & 0x1));	// ACK
	
	// 7 bit addr & 9 bit data
	IICDS = addr<<1 | ((data>>8) & 0x0001);
	IICCON = 0xaf;
	while ((IICCON & 0x10) == 0);	// INT
	while ((IICSTAT & 0x1));	// ACK
		
	IICDS = (data & 0x00FF);
	IICCON = 0xaf;
	while ((IICCON & 0x10) == 0);	// INT
	while ((IICSTAT & 0x1));	// ACK
	
	IICSTAT = 0xd0;	// stop write
	IICCON = 0xaf;
	
	return;
}

void IIC_init(void)
{
	// set GPIO pin function as IICSCL, IICSDA
	GPD1CON |= 0x22;                //GPD1CON[1]  0010 = I2C0_SCL  , GPD1CON[0]  0010 = I2C0_SDA 
	GPD1PUD |= 0x5;              //Pull-down enable
	
	// Enable ACK, Prescaler IICCLK=PCLK/16, Enable interrupt, Transmit clock value Tx clock=IICCLK/16
	IICCON  = (1<<7) | (0<<6) | (1<<5) | (0xf);	
	
	// Enable IIC bus
	//IICADD  = 0x10;                    // no use if s5pv210 is a IIC master
	IICSTAT = 0x10;                    //IIC bus data output enable(Rx/Tx)
}

void WM8960_init(void)
{
#define WM8960_DEVICE_ADDR		0x34
	// reset
	IIC_write(WM8960_DEVICE_ADDR, 0xf, 0x0);

	// power1 2 3
	IIC_write(WM8960_DEVICE_ADDR, 0x19, 1<<8 | 1<<7 | 1<<6);
	IIC_write(WM8960_DEVICE_ADDR, 0x1a, 1<<8 | 1<<7 | 1<<6 | 1<<5 | 1<<4 | 1<<3);
	IIC_write(WM8960_DEVICE_ADDR, 0x2F, 1<<3 | 1<<2);
	
	// clock
	IIC_write(WM8960_DEVICE_ADDR, 0x4, 0x0);		// CLKSEL = 0 : no PLL -> SYSCLK using MCLK 

	// no mute	ok
	IIC_write(WM8960_DEVICE_ADDR, 0x5, 0x0);			// set no mute
	//IIC_write(WM8960_DEVICE_ADDR, 0x5, 0x08);		// set mute 
	
	// audio interface
	IIC_write(WM8960_DEVICE_ADDR, 0x7, 0x2);			// 00 = 16bits,  10 = IIS format
		
	// volume +6db	ok
	IIC_write(WM8960_DEVICE_ADDR, 0x2, 0xFF | 0x100);			// WM8960_LOUT1
	IIC_write(WM8960_DEVICE_ADDR, 0x3, 0xFF | 0x100);			// WM8960_ROUT1

	IIC_write(WM8960_DEVICE_ADDR, 0xa, 0xFF | 0x100);		// Left DAC volume
	IIC_write(WM8960_DEVICE_ADDR, 0xb, 0xFF | 0x100);		// Right DAC volume
	
	// mixer control
	IIC_write(WM8960_DEVICE_ADDR, 0x22, 1<<8 | 1<<7);		// Left output mixer control
	IIC_write(WM8960_DEVICE_ADDR, 0x25, 1<<8 | 1<<7);		// Right output mixer control
	
	return;
}

