PRJ = A-INT-demo
SRC = main.c uart.c lib.c buzzer.c
OBJ = $(SRC:.c=.o) start.o 
RO_BASE = 0x21000000

all:
	armasm start.s -o start.o
	armcc -c $(SRC)
	armlink -first start.o -ro-base $(RO_BASE) $(OBJ) -o $(PRJ).axf
	fromelf -bin $(PRJ).axf -o $(PRJ).bin
	fromelf -c $(PRJ).axf -o $(PRJ).txt
	dir *.bin

clean:
	del *.o
	del *.axf
	del *.bin
	del *.txt