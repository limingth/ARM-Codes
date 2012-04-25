	AREA start, CODE
	
	import mymain
	export start
	ENTRY
	
	; set sp (stack pointer) SVC ģʽ
	; internal SRAM (96K)
	; iRAM: 	0xD0020000 - 0xD0038000
	ldr sp, =0xD0030000
			
	b mymain

	import c_irq_handler
	export IRQ_handler
IRQ_handler
	; set sp_irq
	ldr sp, =0xD0034000
	
	; save context
	sub lr, lr, #4
	stmfd sp!, {r0-r12, lr}
	
	; clear pending bit
	; call ISR (beep)
	bl	c_irq_handler
	
	; restore context
	ldmfd sp!, {r0-r12, pc}^
	
	END