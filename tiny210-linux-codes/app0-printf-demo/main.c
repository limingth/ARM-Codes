#include "led.h"
#include "uart.h"
#include "stdio.h"


int mymain(void)
{
	led_init();
	led_on();
//	uart_init();

	printf("hello, printf demo\n");
	printf("test int 100 = %d \n", 100);
	printf("test hex 100 = 0x%x \n", 100);
	printf("test char 0x31 = %c \n", 0x31);
	printf("test str = %s \n", "bye");

	return 0;
}
