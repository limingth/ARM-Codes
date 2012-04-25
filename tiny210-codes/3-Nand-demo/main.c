#include "uart.h"
#include "lib.h"
#include "nand.h"

int mymain(void)
{
	char id[5];
	char buf[2048];
	int i;
	int addr;
	int nand_addr;
	int sdram_addr;
	
	uart_init();
	
	puts("hello, world!\n");
	
	uart_putchar('0');
	uart_putchar('x');
	putchar_hex(0xE1);	
	
	puts("\nNand chip id is: 0x");
	nand_init();
	nand_read_id(id);
		
	for (i = 0; i < 5; i++)
		putchar_hex(id[i]);
	puts("\n");

#if 0		
	addr = 0x400000;
	nand_read_page(addr, buf);
	putchar_hex(buf[0]);
	putchar_hex(buf[1]);
	putchar_hex(buf[2]);
	putchar_hex(buf[3]);
	puts("\n");
	
	addr += 0x800;
	nand_read_page(addr, buf);
	putchar_hex(buf[0]);
	putchar_hex(buf[1]);
	putchar_hex(buf[2]);
	putchar_hex(buf[3]);
	puts("\n");
	
	addr += 0x800;
	nand_read_page(addr, buf);
	putchar_hex(buf[0]);
	putchar_hex(buf[1]);
	putchar_hex(buf[2]);
	putchar_hex(buf[3]);
	puts("\n");
	while (1);
#endif	
	
#if 0
	nand_addr = 0x0;
	sdram_addr = 0xD0020000;
	nand_read(nand_addr, (char *)sdram_addr, 0x20000);
	((void (*)(void))sdram_addr)();
#endif
	
#if 0
	nand_addr = 0x0;
	sdram_addr = 0xD0020000;
	nand_read(nand_addr, (char *)sdram_addr, 0x2000);
	((void (*)(void))sdram_addr)();
#endif

	nand_addr = 0x4000;		//16K
	sdram_addr = 0x22004000;
	nand_read(nand_addr, (char *)sdram_addr, 0x100000);
	((void (*)(void))sdram_addr)();
	
	// Linux ÄÚºËÔÚ Flash 4M ´¦, size = 4M+ => 5M
	// 0100 0000 0000 0000 0000 0000 
	nand_read(0x00000, (char *)0x20000000, 0x500000);

	md((char *)0x20000000, 16);
	md((char *)0x20000800, 16);	
	md((char *)0x20001000, 16);		
	md((char *)0x20100000, 16);
	md((char *)0x20100800, 16);	
	md((char *)0x20101000, 16);	
	md((char *)0x20200000, 16);
	md((char *)0x20300000, 16);
	md((char *)0x20400000, 16);
	
	//for (i = 0; i < 64; i++)
		//putchar_hex(buf[i]);
	
	addr = 0;	
	while (1)
	{			
		nand_read_page(addr, buf);
		
		//if (buf[0] == 0x63 && buf[1] == 0x74 && buf[2] == 0x45 && buf[3] == 0x44)		
		//if (buf[0] == 0xFF && buf[1] == 0xFF && buf[2] == 0x00 && buf[3] == 0x10
		//	&& buf[4] == 0x00 && buf[5] == 0x00 && buf[6] == 0x01 && buf[7] == 0x00)
		//if (buf[0] == 0x03 && buf[1] == 0x00 && buf[2] == 0x00 && buf[3] == 0x00
			//&& buf[4] == 0x01 && buf[5] == 0x00 && buf[6] == 0x00 && buf[7] == 0x00)
		//if (buf[0] == 0x12 && buf[1] == 0x00 && buf[2] == 0x00 && buf[3] == 0x00	
			//&& buf[4] == 0xf2 && buf[5] == 0x00 && buf[6] == 0x00 && buf[7] == 0x00)
		if (buf[0] == 0x00 && buf[1] == 0x00 && buf[2] == 0xA0 && buf[3] == 0xE1)
		//if (0)
		{
			putint_hex(addr);
			puts(": ");		
			for (i = 0; i < 16; i++)
				putchar_hex(buf[i]);
			puts("\n");
		}
			
		addr += 2048;	
	//	buf += 2048;
		//addr += 0x100000;	
		//delay();
		if (addr > 0x100000*256)
			break;
	}	
	
	uart_putchar('E');
	
	while (1)
	{
		char c = 'a';
		
		c = uart_getchar();
		
		//for (c = 'a'; c <= 'z'; c++)
		uart_putchar(c);
	}

	return 0;
}