
PRJ = a$(shell basename `pwd`)
#PRJ = ${PWD##*/}
SRC = $(wildcard *.c)
OBJ = start.o irq.o $(SRC:.c=.o) 

CROSS = arm-linux-
CC = $(CROSS)gcc
LD = $(CROSS)ld
AS = $(CROSS)as
OBJCOPY = $(CROSS)objcopy
OBJDUMP = $(CROSS)objdump
CFLAGS = -Wall
#LDFLAGS = -Ttext 0xD0030010
LDFLAGS = -Ttext 0x21000000 

all: $(PRJ).bin
	ls -l *.bin

$(PRJ).bin: $(PRJ).elf
	$(OBJCOPY) -O binary $< $@
#	$(OBJDUMP) -d $< > $(PRJ).lst
	$(OBJDUMP) -d -j .text $< > $(PRJ).lst
	$(OBJDUMP) -d -s -j .data -j .rodata $< >> $(PRJ).lst

$(PRJ).elf: $(OBJ)
	$(LD) $(LDFLAGS) $^ -o $@

c clean:
	-rm *.o
	-rm *.elf
	-rm *.bin
	-rm *.lst
	
