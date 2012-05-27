
#define APLL_CON  	(*(volatile unsigned int *)0xe0100100) 
#define CLK_SRC0  	(*(volatile unsigned int *)0xe0100200) 
#define CLK_DIV0  	(*(volatile unsigned int *)0xe0100300) 
#define MPLL_CON  	(*(volatile unsigned int *)0xe0100108)  

void clock_init(void)
{
	// clock init
	APLL_CON = 0xa07d0301;
	CLK_SRC0 = 0x10001111;
	CLK_DIV0 = 0x14131440;
	MPLL_CON = 0xa29b0c01;

	return;
}

