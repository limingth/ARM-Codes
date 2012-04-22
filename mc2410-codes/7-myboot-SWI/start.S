	AREA uartdemo, CODE, READONLY
	
	import mymain
	ENTRY
	
	; exception vector table
	b reset_handler
	nop
	b swi_handler
	nop
	nop
	nop
	b .
	nop
	
reset_handler	
	; set sp_svc stack = 0xF00 (256bytes)
	mov sp, #0xF00
	
	; change mode to IRQ, set sp_irq
	mov r0, #0xD2
	msr cpsr_cxsf, r0
	mov sp, #0xF80
		
	; change mode to USR (0xD0) -> 0x50 (IRQ enable)
	mov r0, #0x50
	msr cpsr_cxsf, r0
	
	; set sp_usr stack = 4K
	mov sp, #0x1000
	
	b mymain
	
	import _sys_call
swi_handler
	; save
	stmfd sp!, {lr}
	
	; r0, r1, r2 (fd, buf, size)
	bl _sys_call
	
	; restore
	ldmfd sp!, {pc}^
	

	import c_irq_handler
irq_handler
	; save
	sub lr, lr, #4
	stmfd sp!, {lr}
	
	bl c_irq_handler
	
	; restore
	ldmfd sp!, {pc}^
		
	END