
PRJ = $(shell basename `pwd`)
#PRJ = ${PWD##*/}
SRC = $(wildcard *.c)
OBJ = start.o mem_setup.o $(SRC:.c=.o) 

CROSS = arm-linux-
CC = $(CROSS)gcc
LD = $(CROSS)ld
AS = $(CROSS)as
OBJCOPY = $(CROSS)objcopy
OBJDUMP = $(CROSS)objdump
CFLAGS = -Wall -O1
#LDFLAGS = -Ttext 0xD0030010
LDFLAGS = -Ttext 0xD0020010

all: $(PRJ).bin
	ls -l *.bin
	make -C bootloader

$(PRJ).bin: $(PRJ).elf
	$(OBJCOPY) -O binary $< $@
#	$(OBJDUMP) -d $< > $(PRJ).lst
	$(OBJDUMP) -d -j .text $< > $(PRJ).lst
	$(OBJDUMP) -d -s -j .data -j .rodata $< >> $(PRJ).lst
	../mktiny210spl.exe $(PRJ).bin $(PRJ)-sd.bin

$(PRJ).elf: $(OBJ)
	$(LD) $(LDFLAGS) $^ -o $@

b burn:
	mount | grep sdb
	#sudo dd iflag=dsync oflag=dsync if=/dev/zero of=/dev/sdb seek=1 count=48
	sudo dd iflag=dsync oflag=dsync if=$(PRJ)-sd.bin of=/dev/sdb seek=1 count=16
	#sudo dd iflag=dsync oflag=dsync if=/dev/zero of=/dev/sdb seek=17 count=32
	sudo dd iflag=dsync oflag=dsync if=bootloader/bootloader.bin of=/dev/sdb seek=1949696

c clean:
	-rm *.o
	-rm *.elf
	-rm *.bin
	-rm *.lst
	make -C bootloader clean
