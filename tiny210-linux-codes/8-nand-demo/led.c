
#define GPJ2CON		(*(volatile unsigned int *)0xE0200280)
#define GPJ2DAT		(*(volatile unsigned int *)0xE0200284)

void led_init(void)
{
	// LED1-4: GPJ2_0, ... GPJ2_3
	// 0001 0001 0001 0001 = output 
	GPJ2CON &= ~0xFFFF;	
	GPJ2CON |= 0x1111;	
	
	return;	
}

void led_on(void)
{
	// set bit 0 -> led on
	GPJ2DAT &= ~0xf;
	
	return;	
}

void led_off(void)
{
	// set bit 1 -> led off
	GPJ2DAT |= 0xf;
	
	return;	
}
