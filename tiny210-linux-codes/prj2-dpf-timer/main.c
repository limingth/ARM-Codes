#include "sdhc.h"
#include "uart.h"
#include "stdio.h"
#include "fat.h"
#include "lib.h"
#include "lcd.h"
#include "audio.h"
#include "shell.h"
#include "timer.h"

int argc = 0;
char * argv[32];
int bmpi = 0;

#define BMP_ARRAY_ADDR	0x21800000
#define BMP_SIZE	(0x80000)
#define WAV_FILE_ADDR	0x23000000

void user_irq_handler(void)
{
	char * p;

	printf("^^^user irq handler\n");

	printf("argc is 0x%x\n", argc);
	printf("argv is 0x%x\n", (int)argv);
	printf("bmpi is 0x%x\n", bmpi);
	printf("irq read file %s\n", argv[bmpi]);
//	size = file_fat_read(argv[bmpi], p, 0x100000);
	p = (char *)(BMP_ARRAY_ADDR + bmpi * BMP_SIZE);

	printf("show %d bmp now...", bmpi);
	lcd_draw_bmp((int)p);
	printf("over!\n");

	bmpi++;
	if (bmpi == argc)
		bmpi = 0;

	return;
}

char buf[1024];
char bmpfilenames[512];
char wavfilename[64];

int mymain(void)
{
	char * p;
	int size = 0;
	int i = 0;

	puts("init begin");
//	uart_init();
	SDHC_Init();
	fat_init();
	puts("sd fat init over");

	lcd_init();
	lcd_clear_screen(0xff0000);
	puts("lcd init ok");

	IIS_init();
	puts("IIS init over");

	IIC_init();	
	puts("IIC init ok");

	WM8960_init();
	puts("WM8960 init ok");

	size = file_fat_read("/boot.ini", buf, 1024);
	buf[size] = '\0';
	printf("boot.ini:\n");
	printf("---\n");
	printf("%s\n", buf);
	printf("---\n");
	printf("file boot.ini size = %d\n", size);

	get_key_value("BMP", buf, bmpfilenames);
	printf("BMP = <%s>\n", bmpfilenames);

	argc = shell_parse(bmpfilenames, argv);
	p = (char *)BMP_ARRAY_ADDR;
	for (i = 0; i < argc; i++)
	{
		printf("bmp[%d] = %s\n", i, argv[i]);
		size = file_fat_read(argv[i], p, 0x100000);
		p = p + BMP_SIZE;
	}

	p = (char *)WAV_FILE_ADDR;
	get_key_value("WAV", buf, wavfilename);
	printf("WAV = <%s>\n", wavfilename);
	size = file_fat_read(wavfilename, p, 0x6400000);
	printf("%s size: %d\n", wavfilename, size);

	bmpi = 0;
	timer_init();
	puts("timer init ok");

	while (1)
	{
		printf("play wav now...");
		audio_play_wav((int)p, size);
		printf("over! again\n");
		ndelay(100000);
	}

	return 0;
}
