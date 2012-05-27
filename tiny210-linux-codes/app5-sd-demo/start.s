
.global _start
	
_start:
	ldr sp, =0x21000000

	@mov r0, #0x53
	@msr	CPSR_cxsf, r0

	b mymain
