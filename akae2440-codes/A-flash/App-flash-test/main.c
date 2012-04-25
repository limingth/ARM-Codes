
#define UTRSTAT0 (*(volatile unsigned int *)0x50000010)    
#define UTXH0	(*(volatile unsigned int *)0x50000020)    

#define NFCONF	(*(volatile unsigned int *)0x4E000000)  
#define NFCONT	(*(volatile unsigned int *)0x4E000004)  
#define NFCMD	(*(volatile unsigned int *)0x4E000008)  
#define NFADDR	(*(volatile unsigned int *)0x4E00000C)  
#define NFDATA	(*(volatile unsigned char *)0x4E000010)  
#define NFSTAT	(*(volatile unsigned int *)0x4E000020)  

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

void nand_init(void)
{	
	// set Timing para
	// HCLK = 100M, 1 hclk = 10ns, 
	// see K9f1208.pdf Page14, Twp > 25ns
	// TACLS = 1, TWRPH0 [10:8] = 2
	NFCONF = 1<<8 | 0<<12;
	
	// Nand Flash init
	//	1. Controller enable
	NFCONT |= 1<<0;
	
	//	2. NandFlash Chip enable (nFCE)
	NFCONT &= ~(1<<1);
}

void nand_read_page(int nand_addr, char * buf)
{
	int i = 0;
	
	// Nand Flash Read
	//	1. Ð´ NFCMMD = 0x00
	NFCMD = 0x00;
	
	//	2. Ð´ 4 ´Î NFADDR
	NFADDR = nand_addr & 0xff;		// A0-A7
	NFADDR = (nand_addr >> 9) & 0xff;	// A9-A16
	NFADDR = (nand_addr >> 17) & 0xff;	// A17-A24
	NFADDR = (nand_addr >> 25) & 0xff;	// A25
	
	//	3. ÂÖÑ¯ NFSTAT£¬¿´ R/nB
	while ((NFSTAT & (1<<0)) == 0)
		;
		
	//	4. ¶Á 512 ´Î NFDATA
	for (i = 0; i < 512; i++)
	{
		*buf++ = NFDATA & 0xFF;
	}
}

void nand_read(int nand_addr, char * buf, int size)
{	
	int pages;
	int i;
	
	if (size <= 0)
		return;
	
	// get how many pages to be read
	pages = (size - 1) / 512 + 1;
	
	for (i = 0; i < pages; i++)
		nand_read_page(nand_addr+i*512, buf+i*512);
}

int mymain(void)
{	
	int i = 0;
	char buf[512];

	nand_init();
		
	nand_read(512, buf, 512);
	
	for (i = 0; i < 16; i++)
		putchar_hex(buf[i]);
	
	uart_putchar('\r');
	uart_putchar('\n');
	
	for (i = 512-16; i < 512; i++)
		putchar_hex(buf[i]);
						
	while (1);
	
	return 0;
}