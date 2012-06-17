
.global asm_IRQ_handler
asm_IRQ_handler:
	@ set sp_irq = iRAM top (sp--)
	ldr sp, =0xD0034000
	
	@ lr = lr - 4
	sub r14, r14, #4
	STMFD r13!, {r1-r12, r14}
	
	bl C_IRQ_handler
	
	LDMFD r13!, {r1-r12, pc}^
