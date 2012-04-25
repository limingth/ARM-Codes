PRJ = xmodem

all:
	armasm start.s
	armcc -c main.c
	armcc -c led.c
	armcc -c uart.c
	armcc -c stdio.c
	armcc -c shell.c
	armcc -c lib.c
	armcc -c command.c
	armcc -c xmodem.c
	armlink start.o main.o led.o uart.o stdio.o shell.o lib.o command.o xmodem.o \
		-first start.o -entry start -ro-base 0x50000000 -o $(PRJ).axf
	fromelf -bin $(PRJ).axf -o $(PRJ).bin
	fromelf -c -d $(PRJ).axf -o $(PRJ).txt
	dir *.bin
	
clean:
	del *.bak
	del *.o
	del *.axf
	del *.bin
	del *.txt