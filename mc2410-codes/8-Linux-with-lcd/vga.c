
#define GPECON    (*(volatile unsigned *)0x56000040) //Port E control
#define GPEDAT    (*(volatile unsigned *)0x56000044) //Port E data
#define GPEUP     (*(volatile unsigned *)0x56000048) //Pull-up control E

#define IICCON  (*(volatile unsigned *)0x54000000) //IIC control
#define IICSTAT (*(volatile unsigned *)0x54000004) //IIC status
#define IICADD  (*(volatile unsigned *)0x54000008) //IIC address
#define IICDS   (*(volatile unsigned *)0x5400000c) //IIC data shift

void IIC_write(int slvAddr, int addr, char data)
{
	IICDS = slvAddr;
	IICSTAT = 0xf0;
	while ((IICCON & 0x10) == 0);	// INT
	while ((IICSTAT & 0x1));	// ACK
	
	IICDS = addr;
	IICCON = 0xaf;
	while ((IICCON & 0x10) == 0);	// INT
	while ((IICSTAT & 0x1));	// ACK
	
	
	IICDS = data;
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
	GPEUP  |= 0xc000;                  //Pull-up disable
	GPECON |= 0xa0000000;                //GPE15:IICSDA , GPE14:IICSCL    
	
	// Enable ACK, Prescaler IICCLK=PCLK/16, Enable interrupt, Transmit clock value Tx clock=IICCLK/16
	IICCON  = (1<<7) | (0<<6) | (1<<5) | (0xf);	
	
	// Enable IIC bus
	IICADD  = 0x10;                    //2410 slave address = [7:1]
	IICSTAT = 0x10;                    //IIC bus data output enable(Rx/Tx)
}

int vga_init(void)
{	
	IIC_init();
	    	
	IIC_write(0xEA, 0x4, 0x20);
	
	return 0;
}