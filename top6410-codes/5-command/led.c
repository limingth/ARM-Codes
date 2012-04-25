#include "led.h"

#define GPNCON 	(*(volatile unsigned int *)0x7F008830)
#define GPNDAT 	(*(volatile unsigned int *)0x7F008834)

#define GPKCON0 (*(volatile unsigned int *)0x7F008800)
#define GPKDAT 	(*(volatile unsigned int *)0x7F008808)
		
void led_init(void)
{
	// only for top6410	
	// LED2 -> GPN6 GPNCON[13:12]	01 = Output  	
	GPNCON |= 1<<12;
	GPNCON &= ~(1<<13);
	
	// only for mini6410
	// LED1 -> GPK4 GPKCON0[19:16] 	0001 = Output  
	GPKCON0 |= 1<<16;
	GPKCON0 &= ~(1<<17 | 1<<18 | 1<<19);	
}

void led_on(void)
{
	// mini6410
	// ÁÁ led1(GPK4)			
	GPKDAT &= ~(1<<4);	// led on	
	
	// top6410
	// ÁÁ led1(GPN6)			
	GPNDAT &= ~(1<<6);	// led on
}

void led_off(void)
{
	// mini6410
	GPKDAT |= 1<<4;		// led off
	
	// top6410
	GPNDAT |= 1<<6;		// led off
}