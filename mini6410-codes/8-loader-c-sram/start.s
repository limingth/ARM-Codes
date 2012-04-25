
	AREA demo, CODE, READONLY
	
	import mymain
	ENTRY

	; 0xc00_0000 to 0xc00_2000 = 8k(SRAM) see S3C6410x.pdf page115
	ldr sp, =0xc002000
	
	b mymain
	
	END