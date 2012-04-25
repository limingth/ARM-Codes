	AREA start, CODE
	
	import mymain
	export start
	ENTRY
	
	; set sp (stack pointer)
	; internal SRAM (96K)
	; iRAM: 	0xD0020000 - 0xD0038000
	ldr sp, =0xD0030000
	
	b mymain	
	
	END