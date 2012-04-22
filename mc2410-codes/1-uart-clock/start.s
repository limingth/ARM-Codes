	AREA uartdemo, CODE, READONLY
	
	import mymain
	ENTRY

	mrc	p15, 0, r1, c1, c0, 0		; read ctrl register 
	orr	r1, r1, #0xc0000000		; Asynchronous  
	mcr	p15, 0, r1, c1, c0, 0		; write ctrl register
	
	; set stack = 4K
	mov r13, #0x1000
	
	b mymain
	
	END