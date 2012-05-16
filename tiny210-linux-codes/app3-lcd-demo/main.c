#include "uart.h"
#include "lcd.h"

#define W	480
#define H	272

#define POS	50
#define RADIUS	20

int mymain(void)
{
	uart_putchar('1');

	lcd_init();
	uart_putchar('2');

	lcd_clear_screen(0xFF0000);	// red	
	uart_putchar('3');

	lcd_draw_hline(H/2, 100, W-100, 0xffffff);	// white
	
	lcd_draw_vline(W/2, 50, H-50, 0xffffff);	// white
	
	lcd_draw_cross(POS, POS, RADIUS, 0x000000);	// black
	lcd_draw_cross(POS, W-POS, RADIUS, 0x000000);
	
	lcd_draw_cross(H-POS, POS, RADIUS, 0x000000);
	lcd_draw_cross(H-POS, W-POS, RADIUS, 0x000000);

	uart_putchar('o');
	uart_putchar('k');

	while (1);
				
	return 0;
}
