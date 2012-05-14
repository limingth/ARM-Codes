
#define GPH2CON		(*(volatile unsigned int *)0xE0200C40)
#define GPH2DAT		(*(volatile unsigned int *)0xE0200C44)

void button_init(void)
{
	// K1 - K4	(see mother board) 
	// GPH2_0 - GPH2_3
	// GPH2CON[0]   [3:0]   0000 = Input   
	// ...
	// GPH2CON[3]   [15:12]   0000 = Input     
	GPH2CON &= ~0xFFFF;
	
	return;	
}

int button_is_down(int which)
{
	// button down -> DAT = 0
	int index = which - 1;
	
	if ((GPH2DAT & (1<<index)) == 0)
		return 1;
	
	return 0; 
}



