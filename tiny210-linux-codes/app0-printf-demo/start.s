
.global _start
	
_start:
	ldr sp, =0xD0030000	
	ldr fp, =0xD0034000	
	
	b mymain
	
