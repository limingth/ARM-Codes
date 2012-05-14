
.global _start

_start:
	ldr r0, =0x1
	ldr r1, =0xe0200280
	str r0, [r1]	
	
loop:
	ldr r0, =0x0
	ldr r1, =0xe0200284
	str r0, [r1]	
	
	bl delay
	
	ldr r0, =0x1
	ldr r1, =0xe0200284
	str r0, [r1]	
	
	bl delay
	
	b loop
	
	
delay:
	ldr r0, =0x100000
go_on:	
	sub r0, r0, #1
	cmp r0, #0
	bne go_on
	
	mov pc, lr		
		
