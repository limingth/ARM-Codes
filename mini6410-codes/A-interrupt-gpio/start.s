;/*
; *	startup.s  --   startup file for swi
; *	
; *	Author: 	li ming <admin@lumit.org>
; *	Date:		2004-8-26
; *	Copyleft:	http://www.lumit.org
; */

	AREA asm_code, CODE

; *********************************************************************	

    EXPORT  ENTRY 
		import mymain
			 
ENTRY

	mov r0, #0xd2               
	msr cpsr_cxsf, r0
	ldr r13, =0x50002000
		
	mov r0,#0xd3
	msr cpsr_cxsf, r0
	ldr r13, =0x50001000
	

	b mymain

; **********************************************************************
; * Jump to the address of the C entry point.
; **********************************************************************

	IMPORT  do_irq
	EXPORT  irq_handler
irq_handler
	sub lr, lr, #4
	STMFD	sp!, {r0-r12, lr}        ; push r0-r12 register file and lr( pc return address )
	mrs	r4, SPSR
	stmfd sp!, {r4}
	
	BL	do_irq										 ; goto C handler
		
	ldmfd sp!, {r4}
	msr	 SPSR_cxsf, r4
	LDMFD	sp!, {r0-r12, pc}^        ; recover r0-r12 and pc from stack, cpsr also
	
	END

; **********************************************************************
; * END OF startup.s
; **********************************************************************
