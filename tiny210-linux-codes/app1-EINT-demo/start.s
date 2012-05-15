
.global _start
	
_start:
	ldr sp, =0xD0028000	

	mov r0, #0x53
	msr	CPSR_cxsf, r0

	b mymain
	@ldr pc, =mymain
	

.global asm_IRQ_handler
asm_IRQ_handler:
	@ set sp_irq = iRAM top (sp--)
	ldr sp, =0xD0034000
	
	@ lr = lr - 4
	sub r14, r14, #4
	STMFD r13!, {r1-r12, r14}
	
	bl C_IRQ_handler
	
	LDMFD r13!, {r1-r12, pc}^
