//#include "led.h"
#include "uart.h"
#include "lib.h"
#include "stdio.h"
//#include "shell.h"
//#include "command.h"
//#include "nand.h"
#include "sdhc.h"

int mymain(void)
{
	int addr;

	uart_init();
	uart_putchar('a');
	putchar('\n');
	//printf("uart ok\n");
	puts("uart ok");
	
	SDHC_Init();
	//printf("sdhc ok\n");
	puts("sdhc ok");
	
	addr = 0x20800000;
	// 0 - MBR
	// (1, 16) - 8K superboot.bin
	// (17, 48) - 16K bootloader.bin
	//SDHC_ReadBlocks(49, 32, addr);
	int blk = 0x1DC000;	// max size is 0x1DC400 blocks

	SDHC_ReadBlocks(blk, 64, addr);		// 32k for bootloader.bin
	//printf("sdhc read ok\n");
	puts("read ok");

#if 0
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
	((void (*)(void)) 0x20800000)();

	return 0;
}
