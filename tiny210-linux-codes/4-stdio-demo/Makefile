
PRJ = $(shell basename `pwd`)
#PRJ = ${PWD##*/}
SRC = $(wildcard *.c)
OBJ = start.o $(SRC:.c=.o) 

CROSS = arm-linux-
CC = $(CROSS)gcc
LD = $(CROSS)ld
AS = $(CROSS)as
OBJCOPY = $(CROSS)objcopy
OBJDUMP = $(CROSS)objdump
CFLAGS = -Wall
LDFLAGS = -Ttext 0xD0020010

all: $(PRJ).bin
	ls -l *.bin

$(PRJ).bin: $(PRJ).elf
	$(OBJCOPY) -O binary $< $@
#	$(OBJDUMP) -d $< > $(PRJ).lst
	$(OBJDUMP) -d -j .text -j .data -j .rodata $< > $(PRJ).lst
	../mktiny210spl.exe $(PRJ).bin $(PRJ)-sd.bin

$(PRJ).elf: $(OBJ)
	$(LD) $(LDFLAGS) $^ -o $@

b burn:
	sudo dd iflag=dsync oflag=dsync if=$(PRJ)-sd.bin of=/dev/sdb seek=1

c clean:
	-rm *.o
	-rm *.elf
	-rm *.bin
	-rm *.lst
	
