all:
	armcc -c main.c
	armlink -ro-base 0x0 main.o -o demo.axf
	fromelf -bin demo.axf -o demo.bin
	fromelf -c demo.axf -o demo.txt
	dir
	
clean:
	del *.o 
	del *.axf