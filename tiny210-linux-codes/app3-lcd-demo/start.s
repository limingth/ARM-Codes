
.global _start
	
_start:
	@ldr sp, =0xD0028000	

	stmfd sp!, {lr}

	bl mymain

	ldmfd sp!, {pc}
