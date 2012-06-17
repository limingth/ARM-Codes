
.global _start
		
_start:
	@ldr sp, =0xD0028000	
	@ldr sp, =0x30000000

	mov r0, #0x53
	msr	CPSR_cxsf, r0

	bl tzpc_init

	stmfd sp!, {lr}

	bl mymain

	ldmfd sp!, {pc}


