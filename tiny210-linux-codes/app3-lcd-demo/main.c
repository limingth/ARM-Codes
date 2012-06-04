#include "uart.h"
#include "lcd.h"

#define W	480
#define H	272

#define POS	50
#define RADIUS	20

void delay(void)
{
	int i = 0;

	for (i = 0; i < 20000000; i++)
		;

	return;
}

int mymain(void)
{
	uart_putchar('1');

	lcd_init();
	uart_putchar('2');

	lcd_clear_screen(0xFF0000);	// red	
	uart_putchar('3');

	lcd_draw_pixel(0, 0, 0xFF0000);

	lcd_draw_hline(H/2, 100, W-100, 0xffffff);	// white

	lcd_draw_vline(W/2, 50, H-50, 0xffffff);	// white

	lcd_draw_pixel(H/2, W/2, 0x000000);
	
	lcd_draw_cross(POS, POS, RADIUS, 0x000000);	// black
	lcd_draw_cross(POS, W-POS, RADIUS, 0x000000);
	
	lcd_draw_cross(H-POS, POS, RADIUS, 0x000000);
	lcd_draw_cross(H-POS, W-POS, RADIUS, 0x000000);

	uart_putchar('o');
	uart_putchar('k');
	uart_putchar('\r');
	uart_putchar('\n');

	//while (1);
	delay();

	lcd_draw_bmp(0x21800000);
//	lcd_clear_screen(0xFFFFFF);	// white
				
	return 0;
}
