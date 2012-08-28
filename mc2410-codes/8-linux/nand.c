#define NFCONF	(*(volatile unsigned int *)0x4e000000)
//#define NFCONT	(*(volatile unsigned int *)0x4e000004)
#define NFCMMD	(*(volatile unsigned char *)0x4e000004)
#define NFADDR	(*(volatile unsigned char *)0x4e000008)
#define NFDATA	(*(volatile unsigned char *)0x4e00000c)
#define NFSTAT	(*(volatile unsigned char *)0x4e000010)

#define PAGE_SIZE 	512

static int nand_read_page(int page, char * buf, int start_addr, int size)
{
	int i = 0;
	
	if (start_addr >= PAGE_SIZE)
		return -1;
		
	if (start_addr + size > PAGE_SIZE)
		size = PAGE_SIZE - start_addr;
		
	// begin to read nand	
	if (start_addr < PAGE_SIZE/2)
		NFCMMD = 0x00;
	else
		NFCMMD = 0x01;
	NFADDR = start_addr % (PAGE_SIZE/2);
	NFADDR = (page & 0xFF);
	NFADDR = ((page >> 8) & 0xFF);
	NFADDR = ((page >> 16) & 0xFF);
	
	while((NFSTAT & 0x1) == 0)
		;
	
	for (i = start_addr; i < start_addr + size; i++)
		*buf++ = NFDATA;
		
	return 0;
}

int nand_read(int flash_addr, char * buf, int size)
{
	int page;
	int from = 0;
	int len;
	
	page = flash_addr / PAGE_SIZE;	// >>9	
	from = flash_addr % PAGE_SIZE;
	len = PAGE_SIZE - from;
	
	do
	{	
		nand_read_page(page, buf, from, len);		
		page++;
		buf += len;
		size -= len;
		
		from = 0;
		if (size < PAGE_SIZE)			
			len = size;
		else
			len = PAGE_SIZE;
	}	while (size > 0);
	
	return 0;
}

#if 0
int nand_read(int flash_addr, char * buf, int size)
{
	int page;
	int from = 0;
	page = flash_addr / PAGE_SIZE;	// >>9	
	from = flash_addr % PAGE_SIZE;
	if (from != 0)
	{
		nand_read_page(page, buf, from, PAGE_SIZE-from);
		page++;
		buf += PAGE_SIZE-from;
		size -= PAGE_SIZE-from;
	}	
	while (size >= PAGE_SIZE)
	{
		nand_read_page(page, buf, 0, PAGE_SIZE);
		page++;
		buf += PAGE_SIZE;
		size -= PAGE_SIZE;
	}	
	if (size > 0)
		nand_read_page(page, buf, 0, size);	
	return 0;
}
#endif

void nand_init(void)
{
	// TWRPH0 [6:4] TWRPH0 duration setting value (0~7)
	// Duration = HCLK * (TWRPH0 + 1)
	NFCONF |= 1<<6;
	
	// Enable/Disable [15] NAND Flash controller enable/disable
	// 0 = Disable NAND Flash Controller
	// 1 = Enable NAND Flash Controller
	NFCONF |= 1<<15;
	
	// TACLS Û1°:8] CLE & ALE duration setting value (1~7)
	// Duration = HCLK * (TACLS + 1)
	//NFCONF |= 1<<8;
	
	// NAND Flash Memory	chip enable
  	//	[11]   NAND flash memory nFCE control
  	//	0 : NAND flash nFCE = L (active)
	//	1 : NAND flash nFCE = H (inactive)
  	//	(After auto-boot, nFCE will be inactive.)	
  	NfCONF &= ~(1<<11);
}
