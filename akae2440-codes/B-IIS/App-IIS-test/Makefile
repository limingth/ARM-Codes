PRJ = IIS

all:
	armasm start.s
	armcc -c main.c
	armlink -ro-base 0x31000000 -first start.o \
		start.o main.o -o $(PRJ).axf
	fromelf -bin $(PRJ).axf -o $(PRJ).bin
	fromelf -c $(PRJ).axf -o $(PRJ).txt
	dir *.bin
	
clean:
	del *.o
	del *.axf
	del *.bin
	del *.txt
	
		