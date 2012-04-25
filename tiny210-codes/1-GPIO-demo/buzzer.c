
#define GPD0CON		(*(volatile unsigned int *)0xE02000A0)
#define GPD0DAT		(*(volatile unsigned int *)0xE02000A4)

void buzzer_init(void)
{
	// buzzer GPD0CON 
	// [0] SET 1	
	GPD0CON |= 1<<0;	
	
	return;	
}

void buzzer_on(void)
{
	// set bit 1 -> buzzer on
	GPD0DAT |= 1<<0;
	
	return;	
}

void buzzer_off(void)
{
	// set bit 0 -> buzzer off
	GPD0DAT &= ~(1<<0);
	
	return;	
}