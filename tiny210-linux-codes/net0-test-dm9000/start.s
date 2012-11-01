
.global _start
	
_start:
	ldr sp, =0xD0030000	
	ldr fp, =0xD0034000	
	

	b mymain
	

.global get_cpsr
get_cpsr:
	mrs r0, cpsr
	mov pc, lr

.global call_svc
call_svc:
	swi 0x0
	svc 0x0
	mov pc, lr
