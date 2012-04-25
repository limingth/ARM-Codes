
	AREA demo, CODE, READONLY
	ENTRY	
	CODE32
	
	export start
start
	; led1-led4: use GPK4-GPK7:	GPKCON0: 0x7f008800 as output
	ldr r0, =0x7f008800
	ldr r1, =0x11110000
	str r1, [r0]
	
	; begin to count led	
	; set init value=1, set led addr: GPKDAT: 0x7f008808
	ldr r0, =0x7f008808	
	
	; let led blink
blink	
	; let led1 on	: GPK4=0
	ldr r1, =0x00000010	
	mvn r1, r1
	str r1, [r0]		; light led
	bl delay				; delay sometime

	; let led2 on : GPK5=0
	ldr r2, =0x00000020	
	mvn r2, r2
	str r2, [r0]		; light led
	bl delay				; delay sometime
	
	; let led3 on : GPK6=0
	ldr r3, =0x00000040	
	mvn r3, r3
	str r3, [r0]		; light led
	bl delay				; delay sometime

	; let led4 on : GPK7=0
	ldr r4, =0x00000080
	mvn r4, r4
	str r4, [r0]		; light led
	bl delay				; delay sometime
	
	b blink					; loop

delay	
	mov r7, #0x100000
loop
	sub r7, r7, #1
	cmp r7, #0
	bne loop
	mov pc, lr
	
	END