# Makefile

all:
	armasm demo.s -o demo.o
	armlink demo.o -ro-base 0x0 -entry start -o demo.axf
	fromelf -bin demo.axf -o demo.bin
	fromelf -c -d -s demo.axf -o demo.txt
	dir demo.bin
	
burn:
	sjf2440 /f:demo.bin
	