#include "sdhc.h"
#include "uart.h"
#include "stdio.h"
#include "fat.h"
#include "lib.h"

int mymain(void)
{
	char buf[128];
	char value[32];

	puts("sd fat init begin\n");
//	uart_init();
	SDHC_Init();
	fat_init();
	puts("sd fat init over\n");

	file_fat_ls("/");

	file_fat_ls("/images");

	file_fat_read("/boot.ini", buf, 128);
	printf("boot.ini:\n%s\n", buf);

	get_key_value("OS", buf, value);
	printf("OS = <%s>\n", value);

	get_key_value("BMP", buf, value);
	printf("BMP = <%s>\n", value);

	get_key_value("WAV", buf, value);
	printf("WAV = <%s>\n", value);

	puts("game over");

	return 0;
}
