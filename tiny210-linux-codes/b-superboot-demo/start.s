.global _start
	
_start:
	ldr sp, =0xD0030000	

	bl clock_init
	bl mem_ctrl_asm_init
	
	b mymain 

