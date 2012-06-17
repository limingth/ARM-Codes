#include "sdhc.h"
#include "uart.h"
#include "stdio.h"
#include "fat.h"
#include "lib.h"
#include "lcd.h"
#include "audio.h"
#include "shell.h"
#include "timer.h"
#include "dma.h"

int argc = 0;
char * argv[32];
int bmpi = 0;

#define BMP_ARRAY_ADDR	0x21800000
#define BMP_SIZE	(0x80000)	// 512K
#define BMP_FB_SIZE	(0x100000)	// 1M = 384K bmp file + 522K fb size
#define WAV_FILE_ADDR	0x23000000

void user_irq_handler(void)
{
	char * p;

//	printf("^^^user irq handler\n");

//	printf("argc is 0x%x\n", argc);
//	printf("argv is 0x%x\n", (int)argv);
//	printf("bmpi is 0x%x\n", bmpi);
//	printf("irq read file %s\n", argv[bmpi]);
//	size = file_fat_read(argv[bmpi], p, 0x100000);
	p = (char *)(BMP_ARRAY_ADDR + bmpi * BMP_FB_SIZE);

	printf("^ dma show bmp[%d] = %s now...", bmpi, argv[bmpi]);
	//lcd_draw_bmp((int)p);
	dma_mem_transfer((int)p+BMP_SIZE, 0x22000000, 480*272*4);
	printf("over!\n");

	bmpi++;
	if (bmpi == argc)
		bmpi = 0;

	return;
}

char buf[1024];
char bmpfilenames[512];
char wavfilenames[512];

void dma_test(void);

#if 0
int mymain(void)
{
	int * p1 = (int *)0xd0020000;
	int * p2 = (int *)0xd0028000;
	char * src = "This is a dma uart test: 0123456789abcdef\n";

	puts("init begin");
	uart_init();

	printf("1. dma test mem transfer\n");
	printf("before dma, p1 = <%x %x>\n", *p1, *(p1+1));
	printf("before dma, p2 = <%x %x>\n", *p2, *(p2+1));

	dma_mem_transfer((int)p1, (int)p2, 6);
	
	printf("after dma, p1 = <%x %x>\n", *p1, *(p1+1));
	printf("after dma, p2 = <%x %x>\n", *p2, *(p2+1));

	// 1: uart	
	printf("\n");
	printf("2. dma test peri uart transfer \n");
	dma_peri_transfer((int)src, 0xe2900020, 1, strlen(src));
	printf("test over!\n");

	while (1);
	return 0;
}
#endif

#if 1
int mymain(void)
{
	char * p;
	int size = 0;
	int i = 0;
	//int mode = 0;
	int wargc;
	char * wargv[10];

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
		printf("bmp[%d] = %s -> fb data now\n", i, argv[i]);
		size = file_fat_read(argv[i], p, 0x100000);
		lcd_draw_bmp_v((int)p, (int)p+BMP_SIZE);
		//lcd_draw_bmp((int)p);
		dma_mem_transfer((int)p+BMP_SIZE, 0x22000000, 480*272*4);
		p = p + BMP_FB_SIZE;
	}
	puts("bmp file -> fb data ok");
	
#if 0
	while (1)
	{
		p = (char *)BMP_ARRAY_ADDR;
		for (i = 0; i < argc; i++)
		{
			printf("mode %d: show bmp[%d] = %s now\n", mode, i, argv[i]);

			if (mode == 0)
				lcd_draw_bmp((int)p);
			if (mode == 1)
				memcpy(0x22000000, p+BMP_SIZE, 480*272*4);
			if (mode == 2)
				dma_mem_transfer(p+BMP_SIZE, 0x22000000, 480*272*4);

			p = p + BMP_FB_SIZE;
			ndelay(0x100000);
		}
		mode = ++mode % 3;
	}
#endif

	bmpi = 0;
	timer_init();
	puts("timer init ok");

	p = (char *)WAV_FILE_ADDR;
	get_key_value("WAV", buf, wavfilenames);
	printf("WAV = <%s>\n", wavfilenames);

	wargc = shell_parse(wavfilenames, wargv);
	while (1)
	{
		for (i = 0; i < wargc; i++)
		{
			size = file_fat_read(wargv[i], p, 0x6400000);
			printf("play %s (size: %d) now ... ", wargv[i], size);
			audio_play_wav((int)p, size);
			printf("over!\n");

			ndelay(10000);
		}
	}

	return 0;
}
#endif
