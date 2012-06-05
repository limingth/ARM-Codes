#include "sdhc.h"
#include "uart.h"
#include "stdio.h"
#include "fat.h"
#include "lib.h"
#include "audio.h"

int mymain(void)
{
	char buf[128];
	char wavfilename[32];
	int size = 0;
	char * p = (char *)0x23000000;

	puts("init begin\n");
//	uart_init();
	SDHC_Init();
	fat_init();
	puts("sd fat init over\n");

	IIS_init();
	puts("IIS init over\n");

	IIC_init();	
	puts("IIC init ok\n");

	WM8960_init();
	puts("WM8960 init ok\n");

	file_fat_read("/boot.ini", buf, 128);
	printf("boot.ini:\n%s\n", buf);

	get_key_value("WAV", buf, wavfilename);
	printf("WAV = <%s>\n", wavfilename);

	size = file_fat_read(wavfilename, p, 0x6400000);
	printf("%s size: %d\n", wavfilename, size);

	printf("play wav now...");
	audio_play_wav((int)p, size);
	printf("over!\n");

	return 0;
}
