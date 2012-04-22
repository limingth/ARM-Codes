PRJ = debug-lcd-vga
SRC = main.c uart.c vga.c lcd.c
OBJ = $(SRC:.c=.o)

all:	
	armasm start.s
	armcc -g -c $(SRC)	
	armlink -first start.o -ro-base 0x31000000 start.o $(OBJ) -o $(PRJ).axf
	fromelf -bin $(PRJ).axf -o $(PRJ).bin
	fromelf -c $(PRJ).axf -o $(PRJ).txt
	dir *.bin
	
clean:
	del *.o 
	del *.axf
	del *.bin
	del *.txt