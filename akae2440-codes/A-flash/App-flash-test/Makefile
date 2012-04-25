all:
	armasm start.s
	armcc -c main.c
	armlink -ro-base 0xC00 -first start.o \
		start.o main.o -o flash.axf
	fromelf -bin flash.axf -o flash.bin
	fromelf -c flash.axf -o flash.txt
	dir *.bin
	
clean:
	del *.o
	del *.axf
	del *.bin
	del *.txt
	
		