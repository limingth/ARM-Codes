	
	AREA demo, CODE, READONLY
	
	import mymain
	ENTRY
	
	mov sp, #0x1000
	
	b mymain
	
	END