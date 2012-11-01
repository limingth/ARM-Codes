#include "led.h"
#include "uart.h"
#include "stdio.h"

void handler(void)
{
	uart_putchar('1');
	printf("handler!\n");

	return;
}

int mymain(void)
{
	char buf[10];
	int cpsr;

	led_init();
	led_on();
//	uart_init();
//
	cpsr = get_cpsr();
	printf("cpsr = %x \n", cpsr);

	printf("hello, printf demo\n");
	printf("test int 100 = %d \n", 100);
	printf("test hex 100 = 0x%x \n", 100);
	printf("test char 0x31 = %c \n", 0x31);
	printf("test str = %s \n", "bye");

#define PREG(x)		printf(#x"= 0x%x \n", *(volatile int *)x )

	PREG(0xE8000000);
	PREG(0xE8000004);
	PREG(0xE8000008);
	PREG(0xE800000c);
	PREG(0xE8000010);
	PREG(0xE8000014);
	PREG(0xE8000018);

	*(int *)0xe8000000 = 0x31;
	PREG(0xE8000000);
		
	dm_init();

	dm_read_id(buf);
	printf("DM9000 id is %x %x %x %x \n", buf[0], buf[1], buf[2], buf[3]);
	delay(5000000);

	while (1)
	{
		printf("send arp ...\n");
		dm_send_arp();

		printf("recv arp ...\n");
		dm_recv_arp();
		delay(1000000);
	}

	return 0;
}
