
all:
	armasm start.s -o start.o
	armcc -c main.c -o main.o
	armlink start.o main.o -first start.o -ro-base 0x50000000 -o demo.axf
	fromelf -bin demo.axf -o demo.bin
	fromelf -c -d -s demo.axf -o demo.txt
	dir *.bin
