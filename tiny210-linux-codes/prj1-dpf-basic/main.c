#include "sdhc.h"
#include "uart.h"
#include "stdio.h"
#include "fat.h"
#include "lib.h"
#include "lcd.h"
#include "audio.h"
#include "shell.h"

int mymain(void)
{
	char buf[128];
	char bmpfilenames[32];
	char wavfilename[32];
	int size = 0;
	char * p = (char *)0x23000000;

	int argc = 0;
	char * argv[32];
	int i = 0;

	puts("init begin");
//	uart_init();
	SDHC_Init();
	fat_init();
	puts("sd fat init over");

	lcd_init();
	puts("lcd init ok");

	IIS_init();
	puts("IIS init over");

	IIC_init();	
	puts("IIC init ok");

	WM8960_init();
	puts("WM8960 init ok");

	file_fat_read("/boot.ini", buf, 128);
	printf("boot.ini:\n%s\n", buf);

	get_key_value("BMP", buf, bmpfilenames);
	printf("BMP = <%s>\n", bmpfilenames);

	argc = shell_parse(bmpfilenames, argv);
	while (1)
	{
		for (i = 0; i < argc; i++)
		{
			size = file_fat_read(argv[i], p, 0x100000);
			printf("%s size: %d\n", argv[i], size);

			printf("show bmp now...");
			lcd_draw_bmp((int)p);
			printf("over!\n");

			ndelay(0x800000);
		}
	}

	get_key_value("WAV", buf, wavfilename);
	printf("WAV = <%s>\n", wavfilename);
	size = file_fat_read(wavfilename, p, 0x6400000);
	printf("%s size: %d\n", wavfilename, size);

	printf("play wav now...");
	audio_play_wav((int)p, size);
	printf("over!\n");

	return 0;
}
