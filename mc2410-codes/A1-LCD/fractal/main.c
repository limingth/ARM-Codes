#include "uart.h"
#include "lcd.h"

void vga_init(void);

#define BMP_ADDR	(0x33000000)
#define ROW 	480
#define COL 	640

// x -> [-320, 320] [0, 640]
// y -> [240, -240] [0, 480]

#define FB_ADDR		(0x32000000)	

//#define lcd_draw_pixel(a, b, c)		*(short *)(FB_ADDR + ((a)*640+(b))*2) = (c)

#define draw_xy(a, b, c)	lcd_draw_pixel(240-b, a+320, c)

#if 0
void draw_xy(int x, int y, int color)
{
	lcd_draw_pixel(480-(y+240), x+320, color);
}
#endif

int get_fratal(double x, double y)
{
	int n = 0;
	double nx = 0, ny = 0;
	int i;
	// Z = x + yi 
	// Z*Z = (x*x - y*y) + 2*x*y i
	// Z <- Z*Z - 0.75
	
	//while (nx < 1 && ny < 1)
	for (i = 0; i < 30; i++)
	{
		nx = nx*nx - ny*ny + x;
		ny = 2*nx*ny + y;
		
		n++;
		
		if ((nx*nx + ny*ny) > 4)
			break;
	}
	
	return ( i * 100000);
}

int mymain(void)
{	
	char * p = (char *)BMP_ADDR;
	char red, blue, green;
	int color;
	int i, j;
	
	vga_init();

	lcd_init();
	
	lcd_clear_screen(0x000000);	// black	
	
	draw_xy(0, 0, 0xff0000);
	draw_xy(0, 200, 0x00ff00);
	draw_xy(300, 0, 0x0000ff);

	for (i = -320; i < 320; i++)
		for (j = -240; j < 240; j++)
		{
			double x, y;
			int color = 0xff0000;
			
			x = (double)i / (double)150;
			y = (double)j / (double)150;
			
			color = get_fratal(x, y);
			
			draw_xy(i, j, color);
			//printf("i = %d, j = %d, color = %d \n", i, j, color);
		}
		
	while(1);
	

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