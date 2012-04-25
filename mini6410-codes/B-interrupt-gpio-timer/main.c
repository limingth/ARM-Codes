	
#define GPKCON0 (*(volatile unsigned int *)0x7f008800)
#define GPKDAT (*(volatile unsigned int *)0x7f008808)
	
#define GPNCON (*(volatile unsigned int *)0x7F008830)
#define GPNDAT (*(volatile unsigned int *)0x7F008834)

#define EINT0CON0 (*(volatile unsigned int *)0x7F008900)
 

void delay(void)
{
	int i;
	for( i = 0x100000; i >= 0; i-- )
		;
	
	return;
}

#define EINT0MASK (*(volatile unsigned int *)0x7F008920)
#define EINT0PEND (*(volatile unsigned int *)0x7F008924)

#define VIC0ADDRESS (*(volatile unsigned int *)0x71200F00)


#define TCFG0 (*(volatile unsigned int *)0x7F006000)
#define TCON (*(volatile unsigned int *)0x7F006008)
#define TCNTB0 (*(volatile unsigned int *)0x7F00600C)
#define TCMPB0 (*(volatile unsigned int *)0x7F006010)
  
#define TINT_CSTAT (*(volatile unsigned int *)0x7F006044)
    
	 
void isr1(void)
{	
	// buzzer on
#define GPFCON (*(volatile unsigned int *)0x7F0080A0)
#define GPFDAT (*(volatile unsigned int *)0x7F0080A4)
	
	// set GPF14 as output
	GPFCON |= 1<<28;
	GPFCON &= ~(1<<29);
	
	GPFDAT |= 1<<14;
	delay();
	GPFDAT &= ~(1<<14);	
}

void isr2(void)
{
	// led blink
	GPKDAT = 0x0;	
	delay();
	GPKDAT = 0x90;	
	delay();
}

void isr3(void)
{
	// led blink
	GPKDAT = 0x0;	
	delay();
	GPKDAT = 0x60;	
	delay();
}

void do_irq(void)
{
	void (*pf)(void);
	int addr;

	EINT0PEND = 1<<0 | 1<<4;	
	TINT_CSTAT = 1<<5;
	
	addr = VIC0ADDRESS;

	pf = (void (*)(void))addr;
	pf();
}

void install_irq_handler( void (*isr)(void) )
{
	/* ARM irq exception vector addr is 0x00000018  */
	unsigned int * irq_vec_addr = ( unsigned int * ) 0x18;
	/* this is isr entry address, could be another address like 0x3c, 0x58... */
	unsigned int * isr_entry_addr = ( unsigned int * ) 0x38;

	unsigned int instruction;
	
	/* set the ISR entry at 0x38  */
	*isr_entry_addr = (unsigned int)isr;
	
	/* make an instruction: it is machine-code for "ldr  pc, [pc, #(38-18-8)]"  */
	instruction = ((unsigned int) isr_entry_addr  - (unsigned int)irq_vec_addr - 0x08) | 0xe59ff000;
	
	/* set this instruction at 0x18  */
	*irq_vec_addr = instruction;	
	
	return;
}

void irq_handler(void);

int mymain(void)
{
	// set GPKCON0: output	(led1-led4: use GPK4-GPK7)
	GPKCON0 = 0x11110000;	
	
	// set all led off (write 1 to be off)
	GPKDAT = 0x000000f0;

	// set GPN0 as ext. interrupt 0	(10=Ext. Interrupt[0])
	GPNCON = 1<<1 | 1<<9;
	
	TCFG0 = 199;	
	TCNTB0 = 65535;
	TCMPB0 = 60000;
	TINT_CSTAT = 1<<0;
 	TCON   = (1<<1);   // 手动更新
	TCON   = 0x0d;     // 自动加载，清“手动更新”位，启动定时器0
	
	// Set EINT0 interrupt evoke mode (01x = Falling edge triggered)
	EINT0CON0 = 1<<1 | 1<<9;

#define VIC0IRQSTATUS (*(volatile unsigned int *)0x71200000)
#define VIC0RAWINTR (*(volatile unsigned int *)0x71200008)
#define VIC0INTENABLE (*(volatile unsigned int *)0x71200010)

#define VIC0VECTADDR0 (*(volatile unsigned int *)0x71200100)
#define VIC0VECTADDR1 (*(volatile unsigned int *)0x71200104)

#define VIC0VECTADDR23 (*(volatile unsigned int *)(0x71200100+23*4))

	VIC0VECTADDR0 = (unsigned int)isr2;
	VIC0VECTADDR1 = (unsigned int)isr3;
	VIC0VECTADDR23 = (unsigned int)isr2;
	
	install_irq_handler(irq_handler);
 	 
	EINT0MASK &= ~(1<<0 | 1<<4);
	
	VIC0INTENABLE = 1<<0 | 1<<1 | 1<<23;
	
	__asm
	{
		MOV	r0,#0x53
		MSR	CPSR_cxsf, r0
				
	}
	
	while(1)
	{
		if (EINT0PEND)
		{
			GPKDAT = 0x0;	
			delay();
			EINT0PEND = 1<<0;
		}
		else
		{	
			GPKDAT = 0x00000070;
			delay();
			GPKDAT = 0x000000f0;
			delay();	
		}
	}
	
	return 0;	
}