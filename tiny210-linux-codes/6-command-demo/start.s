
.global _start
	
_start:
	ldr sp, =0xD0030000	
	
	b mymain
	
