
.global _start
	
_start:
	@ iRAM from 0xD00020000 to 0xD0038000
	ldr sp, =0xD0038000	
	
	b mymain
	
