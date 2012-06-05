#include "sdhc.h"
#include "uart.h"
#include "stdio.h"
#include "fat.h"
#include "lib.h"
#include "lcd.h"

int mymain(void)
{
	char buf[128];
	char bmpfilename[32];
	int size = 0;
	char * p = (char *)0x21800000;

	puts("init begin\n");
//	uart_init();
	SDHC_Init();
	fat_init();
	puts("sd fat init over\n");

	lcd_init();
	puts("lcd init ok\n");

	file_fat_read("/boot.ini", buf, 128);
	printf("boot.ini:\n%s\n", buf);

	get_key_value("BMP", buf, bmpfilename);
	printf("BMP = <%s>\n", bmpfilename);
	size = file_fat_read(bmpfilename, p, 0x6400000);
	printf("%s size: %d\n", bmpfilename, size);

	printf("show bmp now...");
	lcd_draw_bmp((int)p);
	printf("over!\n");

	return 0;
}
