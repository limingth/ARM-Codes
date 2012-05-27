#include "led.h"
#include "uart.h"
#include "lib.h"
#include "stdio.h"
#include "shell.h"
#include "command.h"
#include "nand.h"
#include "210sdhc.h"
#include "fat.h"
#include "lcd.h"


int mymain(void)
{
	char * autocmd[] = {"sdload tian.bmp 0x21800000", "draw 0x21800000", "sdload today.wav 0x23000000", "play 0x23000000"};

	char buf[64];
	int argc = 0;
	char * argv[10];
	int i = 0;
	int counter = 0;

	led_init();
	led_on();

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
	//lcd_clear_screen(0xFF0000);	// red	
	//lcd_draw_hline(H/2, 100, W-100, 0xffffff);	// white
	//lcd_draw_vline(W/2, 50, H-50, 0xffffff);	// white

	printf("\n\n" __DATE__ "  " __TIME__ "\n");
	printf("welcome to my boot v2.0 \n\n");
	
	//printf("test: %c,%s,%d,%x\n", 'A', "abcdef", 11, 0x23);

	while (1)
	{
		printf("LUMIT $ ");

		if (counter < 4)
			strcpy(buf, autocmd[counter]);
		else		
			gets(buf);

		counter++;
		
		printf("your input: <%s>\n", buf);
			
		argc = shell_parse(buf, argv);
		
		for (i = 0; i < argc; i++)
			printf("%d: <%s>\n", i, argv[i]);	

		command_do(argc, argv);
	}
}
