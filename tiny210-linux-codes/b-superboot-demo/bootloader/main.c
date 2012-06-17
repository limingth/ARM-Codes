#include "led.h"
#include "uart.h"
#include "lib.h"
#include "stdio.h"
#include "shell.h"
#include "command.h"
#include "nand.h"
#include "sdhc.h"
#include "fat.h"
#include "lcd.h"
#include "audio.h"

int mymain(void)
{
	char buf[128];
	int argc = 0;
	char * argv[10];
	int i = 0;

	int flag = 0;
	char ini_buf[128];

	led_init();
	led_on();

	printf("init ...");
	uart_init();

	nand_init();
	nand_read_id(buf);
	putchar_hex(buf[0]);
	putchar_hex(buf[1]);
	putchar_hex(buf[2]);
	putchar_hex(buf[3]);
	putchar_hex(buf[4]);
	
	SDHC_Init();

	fat_init();

	IIC_init();	
	
	WM8960_init();
	
	IIS_init();

#define W	480
#define H	272
	lcd_init();
	lcd_clear_screen(0xFF0000);	// red	
	lcd_draw_hline(H/2, 100, W-100, 0xffffff);	// white
	lcd_draw_vline(W/2, 50, H-50, 0xffffff);	// white
	
	printf("ok!");

	printf("\n\n" __DATE__ "  " __TIME__ "\n");
	printf("welcome to my boot v2.0 \n\n");
	
	//printf("test: %c,%s,%d,%x\n", 'A', "abcdef", 11, 0x23);

	printf("please enter a key to shell mode in 3 seconds... \n");
	
	while (i < 0x400000)
	{
		if ((UTRSTAT0 & (1<<0)) == 1)
		{
			flag = 1;
			break;
		}
		
		if ( (i & 0x0fffff) == 0 )
			printf("%d ", 3 - (i>>20));
			
		i++;
	}
	
	if (flag == 0)	
	{
		char binfilename[32];
		int size = 0;

		printf("auto cmd line is performed!\n");

		file_fat_read("/boot.ini", ini_buf, 128);
		printf("boot.ini:\n%s\n", ini_buf);

		get_key_value("OS", ini_buf, binfilename);
		size = file_fat_read(binfilename, (char *)0x21000000, 0x10000);		// 64K
		printf("%s size: %d\n", binfilename, size);
		((void (*)(void))0x21000000)();
	}

	while (1)
	{
		printf("LUMIT $ ");

		gets(buf);

		printf("your input: <%s>\n", buf);
			
		argc = shell_parse(buf, argv);
		
		for (i = 0; i < argc; i++)
			printf("%d: <%s>\n", i, argv[i]);	

		command_do(argc, argv);
	}
}
