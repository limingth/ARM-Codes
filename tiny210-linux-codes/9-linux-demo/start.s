
.global _start
	
_start:
	ldr sp, =0xD0028000	

sdram_init:
	bl mem_ctrl_asm_init


	ldr r0, =0xD0020000
	ldr r1, =0xD0030000
	ldr r2, =0x2000
move_loop:
	ldr r3, [r0]
	str r3, [r1]
	add r0, r0, #4
	add r1, r1, #4
	sub r2, r2, #4
	cmp r2, #0
	bne move_loop

	/* b mymain */
	ldr pc, =mymain
	
