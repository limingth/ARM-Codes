	
	AREA demo, CODE, READONLY
	
	import mymain
	import sys_init
	ENTRY
	
;	mov sp, #0x1000

	mov sp, #0x34000000	; SDRAM
	
	bl sys_init
	
	b mymain
	
	END