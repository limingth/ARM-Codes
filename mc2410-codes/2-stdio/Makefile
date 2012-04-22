PRJ = 2-stdio

all:
	armasm start.s
	armcc -c main.c
	armcc -c uart.c
	armcc -c stdio.c	
	armlink start.o main.o uart.o stdio.o -first start.o -ro-base 0x0 -o $(PRJ).axf
	fromelf -bin $(PRJ).axf -o $(PRJ).bin
	fromelf -c $(PRJ).axf -o $(PRJ).txt
	dir *.bin
	
clean:
	del *.o
	del *.axf
	del *.bin
	del *.txt
	del *.bak