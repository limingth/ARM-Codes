	AREA uartdemo, CODE, READONLY
	
	import mymain
	ENTRY
	
	; set stack = 4K
	mov r13, #0x1000
	
	b mymain
	
	END