
PRJ = 1$(shell basename `pwd`)
SRC = $(wildcard *.c)
OBJ = start.o $(SRC:.c=.o) 

CROSS = arm-linux-
CC = $(CROSS)gcc
LD = $(CROSS)ld
AS = $(CROSS)as
OBJCOPY = $(CROSS)objcopy
OBJDUMP = $(CROSS)objdump
CFLAGS = -Wall
#LDFLAGS = -Ttext 0xD0020010
LDFLAGS = -Ttext 0x21000000

all: $(PRJ).bin
	ls -l *.bin

$(PRJ).bin: $(PRJ).elf
	$(OBJCOPY) -O binary $< $@
#	$(OBJDUMP) -d $< > $(PRJ).lst
	$(OBJDUMP) -d -j .text -j .data -j .rodata $< > $(PRJ).lst

$(PRJ).elf: $(OBJ)
	$(LD) $(LDFLAGS) $^ -o $@

c clean:
	-rm *.o
	-rm *.elf
	-rm *.bin
	-rm *.lst
	
