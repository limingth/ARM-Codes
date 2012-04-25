
all:
	armasm start.s -o start.o
	armcc -c main.c -o main.o
	armcc -c my_printf.c -o my_printf.o
	armcc -c my_shell.c -o my_shell.o	
	armcc -c my_stdio.c -o my_stdio.o	
	armcc -c my_xmodem.c -o my_xmodem.o		
	armlink start.o main.o my_printf.o my_shell.o my_stdio.o my_xmodem.o -first start.o -ro-base 0x50000000 -o demo.axf
	fromelf -bin demo.axf -o demo.bin
	fromelf -c -d -s demo.axf -o demo.txt
	dir *.bin
