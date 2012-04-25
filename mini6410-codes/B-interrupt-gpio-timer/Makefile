all:
	armasm start.s -o start.o
	armcc -c main.c -o main.o
	armlink start.o main.o -first start.o -entry 0x50000000 -ro-base 0x50000000 -o interrupt.axf
	fromelf -bin interrupt.axf -o interrupt.bin
	fromelf -c -d -s interrupt.axf -o interrupt.lst
	
clean:
	del -f *.o
	del -f *.axf
	del -f *.lst
	del -f *.bin
	del -f *.bak

	