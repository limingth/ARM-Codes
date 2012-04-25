PRJ = uart

all:
	armasm start.s
	armcc -c main.c
	armcc -c led.c
	armcc -c uart.c
	armlink start.o main.o led.o uart.o -first start.o -entry start -ro-base 0x50000000 -o $(PRJ).axf
	fromelf -bin $(PRJ).axf -o $(PRJ).bin
	fromelf -c $(PRJ).axf -o $(PRJ).txt
	dir *.bin
	
clean:
	del *.bak
	del *.o
	del *.axf
	del *.bin
	del *.txt