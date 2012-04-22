#include "stdio.h"
#include "lib.h"
#include "xmodem.h"
#include "nand.h"

//#define DOWNLOAD_ADDR	0xC00
#define DOWNLOAD_ADDR	0x31000000

void help(int argc, char * argv[])
{
	printf("help: \n");
	printf("md: md 0x0\n");
	printf("mw: mw 0x0 0x1122\n");
	printf("load: load 0x31000000 132\n");
	printf("loadx: loadx\n");
	printf("go: go 0x31000000\n");
	printf("flashw: flashw 0x0 0x30000000 0x10000\n");
	printf("flashl: flashl 0x0 0x30000000 0x10000\n");
}	

void md(int argc, char * argv[])
{	
	int addr;
	int * p;
	int i = 0; 
	int j = 0;
	
	printf("md usage: md 0x0\n");
	
	addr = atoi(argv[1]);
	p = (int *)addr;	
	
	for (i = 0; i < 16; i++)
	{		
		printf("%x:", addr + i*16);
		
		for (j = 0; j < 4; j++)
			printf(" %x", *p++);

		printf("\n");
	}		
}

void mw(int argc, char * argv[])
{	
	int addr;
	int value;
	int * p;
	
	printf("mw usage: mw 0x30000000 0xFF\n");
	
	addr = atoi(argv[1]);	
	value = atoi(argv[2]);
	
	// value -> [addr]
	p = (int *)addr;	
	*p = value;
}

void load(int argc, char * argv[])
{	
	int size = 132;
	int addr = DOWNLOAD_ADDR;
	char * p;
	int i;
	
	printf("load usage: load <addr> <size>\n");
		
	if (argc > 2)
	{
		addr = atoi(argv[1]);
		size = atoi(argv[2]);
	}
	
	printf("Ready for binary download to 0x%x \n", addr, size);
	
	// get user load addr 
	p = (char *)addr;
	for (i = 0; i < size; i++)
	{
		*p++ = getchar();
	}
	
	printf("load to addr 0x%x, total size 0x%x \n", addr, size);	
}

void go(int argc, char * argv[])
{		
	int addr = DOWNLOAD_ADDR;
	void (*pf)(void);
		
	printf("go usage: go <addr> \n");
	
	if (argc > 1)
	{
		addr = atoi(argv[1]);		
	}
	
	pf = (void (*)(void))addr;
	
	printf("go to addr: %x\n", addr);
	
	pf();
}


void loadx(int argc, char * argv[])
{		
	int addr = DOWNLOAD_ADDR;
	int size = 0;
	
	printf("loadx usage: loadx 0x31000000 \n");
	
	if (argc > 1)
	{
		addr = atoi(argv[1]);
	}

	size = xmodem_recv(addr);
	
	printf("loadx to addr 0x%x, total size 0x%x \n", addr, size);	
}


/* program nand_flash */
void flashw(int argc, char *argv[])
{
	unsigned char *write_addr, *download_addr;
	unsigned int len;
	unsigned int block;
	unsigned int pages = 0, page_offset = 0;
	int i;
	
	if (argc < 4)
		return ;
		
	write_addr = (unsigned char *)atoi(argv[1]);
	download_addr = (unsigned char *)atoi(argv[2]);
	len = atoi(argv[3]);

	/* flash_write_address < myboot_address */ 
	if ((int)write_addr < 0x1000)
		return ;
	
	/* each block size : 4K = 0x4000 */
	block = ((unsigned int)write_addr) >> (8+1+5);

	/* each page size : 512B */
	pages = len/512;
	page_offset = len%512;
	for (i = 0; i < pages; i++) {	
		/* erase before program for nand_flash */
		if (i == 0)
			nand_blockerase(block);
			
		/* each block 32 pages */
		if (i >= 32) {
			block++;
			i = 0;
			pages -= 32;
		}
		
		nand_program(block, download_addr, i);
		download_addr += 512;
	}
	
	/* last page */
	if (i >= 32) {
		block++;
		i = 0;
	}
	if (page_offset > 0) {
		/* erase before program for nand_flash */
		if (i == 0)
			nand_blockerase(block);
			
		nand_program(block, download_addr, i);
	}
}

/* load from nand_flash */
void flashl(int argc, char *argv[])
{
	unsigned char *read_addr, *download_addr;
	unsigned int len;
	unsigned int block;
	unsigned int pages = 0, page_offset = 0;
	int i;
	
	if (argc < 4)
		return ;
		
	read_addr = (unsigned char *)atoi(argv[1]);
	download_addr = (unsigned char *)atoi(argv[2]);
	len = atoi(argv[3]);
	
	/* each block size : 16K = 0x4000 */
	block = ((unsigned int)read_addr) >> (8+1+5);

	/* each page size : 512B */
	pages = len/512;
	page_offset = len%512;
	for (i = 0; i < pages; i++) {	
		/* each block 32 pages */
		if (i >= 32) {
			block++;
			i = 0;
			pages -= 32;
		}
		
		nand_readpage(block, i, download_addr);
		download_addr += 512;
	}
	
	/* last page */
	if (i >= 32) {
		block++;
		i = 0;
	}
	if (page_offset > 0) {
		nand_readpage(block, i, download_addr);
	}
}
