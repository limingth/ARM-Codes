#include "uart.h"
#include "lcd.h"

void vga_init(void);

#define BMP_ADDR	(0x33000000)

int mymain(void)
{	
	char * p = (char *)BMP_ADDR;
	char red, blue, green;
	int color;
	int i, j;
	
	vga_init();

	lcd_init();
	
	lcd_clear_screen(0x000000);	// black	

	lcd_draw_hline(100, 100, 640-100, 0xff0000);	// red
	lcd_draw_hline(200, 100, 640-100, 0x00ff00);	// green
	lcd_draw_hline(300, 100, 640-100, 0x0000ff);	// blue
	lcd_draw_hline(400, 100, 640-100, 0xffffff);	// white

	lcd_draw_vline(640/2, 50, 480-50, 0xffffff);	// white
	
	#define POS	50
	#define HALF	20
	lcd_draw_cross(POS, POS, HALF);
	lcd_draw_cross(POS, 640-POS, HALF);
	
	lcd_draw_cross(480-POS, POS, HALF);
	lcd_draw_cross(480-POS, 640-POS, HALF);
	
	//while (1);
	
	// show BMP file
		// read bmp file
	// bmp file header is 54 bytes
	p += 54;
	
	for (i = 0; i < 272; i++)
		for (j = 0; j < 480; j++)
		{
			blue = *p++;
			green = *p++;
			red = *p++;
		
			color = red << 16 | green << 8 | blue << 0;
			
			lcd_draw_pixel(272-i, j, color);
		}
	
	while (1);
	
	return 0;
}