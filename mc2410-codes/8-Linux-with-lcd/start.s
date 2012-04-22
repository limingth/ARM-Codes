	area led, code, readonly
	import mymain
	
	
	ENTRY
	
	mov sp, #0x1000
	
	; clear ZI section (BSS section)	
	import |Image$$ZI$$Base|
	import |Image$$ZI$$Limit|

	LDR r3, =|Image$$ZI$$Base| 	
 	LDR r1, =|Image$$ZI$$Limit| 	; Top of zero init segment
	MOV r2, #0
loop_zero
 	CMP r3, r1 						; Zero init
	STRCC r2, [r3], #4
	BCC loop_zero
	
	b mymain
	end
