//#include "led.h"
#include "uart.h"
#include "lib.h"
#include "stdio.h"
//#include "shell.h"
//#include "command.h"
//#include "nand.h"
#include "sdhc.h"

#define BL2_SDRAM_ADDR	0x20A00000

int mymain(void)
{
	uart_init();
	uart_putchar('a');
	putchar('\n');
	//printf("uart ok\n");
	puts("uart ok");
	
	SDHC_Init();
	//printf("sdhc ok\n");
	puts("sdhc ok");
	
	// 0 - MBR
	// (1, 16) - 8K superboot.bin
	// (17, 48) - 16K bootloader.bin
	//SDHC_ReadBlocks(49, 32, addr);
	int blk = 0x1DC000;	// max size is 0x1DC400 blocks

	SDHC_ReadBlocks(blk, 64, BL2_SDRAM_ADDR);		// 32k for bootloader.bin
	//printf("sdhc read ok\n");
	puts("read ok");

#if 1
	int addr = BL2_SDRAM_ADDR;
	//printf("%x: %x \n", addr, *(int *)addr);
	putint_hex(*(int *)addr);
	addr += 4;
	putint_hex(*(int *)addr);
	addr += 4;
	putint_hex(*(int *)addr);
	addr += 4;
	putint_hex(*(int *)addr);
#endif
	// jump to add to execute bootloader.bin
	puts("jump now");
	((void (*)(void)) BL2_SDRAM_ADDR)();

	return 0;
}
