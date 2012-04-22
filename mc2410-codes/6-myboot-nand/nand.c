#define NFCONF	(*(volatile unsigned int *)0x4e000000)
//#define NFCONT	(*(volatile unsigned int *)0x4e000004)
#define NFCMMD	(*(volatile unsigned char *)0x4e000004)
#define NFADDR	(*(volatile unsigned char *)0x4e000008)
#define NFDATA	(*(volatile unsigned char *)0x4e00000c)
#define NFSTAT	(*(volatile unsigned char *)0x4e000010)

void nand_init(void)
{
	//NFCONF = 0x1400;
	//NFCONT = 0x01;
	
	// TWRPH0 [6:4] TWRPH0 duration setting value (0~7)
	// Duration = HCLK * (TWRPH0 + 1)
	NFCONF |= 1<<6;
	
	// Enable/Disable [15] NAND Flash controller enable/disable
	// 0 = Disable NAND Flash Controller
	// 1 = Enable NAND Flash Controller
	NFCONF |= 1<<15;
	
	// TACLS [10:8] CLE & ALE duration setting value (0~7)
	// Duration = HCLK * (TACLS + 1)
	//NFCONF |= 1<<8;
	
	// NAND Flash Memory	chip enable
  	//	[11]   NAND flash memory nFCE control
  	//	0 : NAND flash nFCE = L (active)
	//	1 : NAND flash nFCE = H (inactive)
  	//	(After auto-boot, nFCE will be inactive.)	
  	NFCONF &= ~(1<<11);
}

void nand_reset(void)
{
	NFCMMD = 0xff;
	while (!(NFSTAT & 0x01));
}

int nand_readpage(unsigned int block, unsigned int page, unsigned char *buf)
{
	int addr = (block<<(8+5)) | (page<<8);
	
	int i = 0;

	NFCMMD = 0x00;
	NFADDR = (unsigned char)addr; 
	NFADDR = (unsigned char)(addr>>8); 
	NFADDR = (unsigned char)(addr>>16); 
	NFADDR = (unsigned char)(addr>>24); 
	while (!(NFSTAT & 0x01));
	for (i=0; i<512; i++) {
		*buf++ = NFDATA;
	}
	
	return i;
}

int nand_program(unsigned int block, unsigned char *buf, unsigned int page)
{
	int addr = (block<<(8+5)) | (page<<8);
	int i = 0;

	NFCMMD = 0x80;
	NFADDR = (unsigned char)addr; 
	NFADDR = (unsigned char)(addr>>8); 
	NFADDR = (unsigned char)(addr>>16); 
	NFADDR = (unsigned char)(addr>>24); 
	
	for (i=0; i<512; i++) {
		NFDATA = *buf++;
	}
	NFCMMD = 0x10;
	
	while (!(NFSTAT & 0x01));
	
	return i;
}

void nand_blockerase(unsigned int block)
{
	int addr = (block<<(8+5));

	NFCMMD = 0x60;
	NFADDR = (unsigned char)(addr>>8); 
	NFADDR = (unsigned char)(addr>>16); 
	NFADDR = (unsigned char)(addr>>24); 
	NFCMMD = 0xd0;
	while (!(NFSTAT & 0x01));
}