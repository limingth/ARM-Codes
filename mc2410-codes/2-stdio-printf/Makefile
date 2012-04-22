PRJ = 2-stdio-printf

all:
	armasm start.s
	armcc -c main.c
	armcc -c uart.c
	armcc -c stdio.c
	armcc -c printf.c
	armlink start.o main.o uart.o stdio.o printf.o -first start.o -ro-base 0x0 -o $(PRJ).axf
	fromelf -bin $(PRJ).axf -o $(PRJ).bin
	fromelf -c $(PRJ).axf -o $(PRJ).txt
	dir *.bin
	
clean:
	del *.o
	del *.axf
	del *.bin
	del *.txt
	del *.bak