
PRJ = 2-GPIO-demo
SRC = $(wildcard *.c)
OBJ = start.o $(SRC:.c=.o) 

CROSS = arm-linux-
CC = $(CROSS)gcc
LD = $(CROSS)ld
AS = $(CROSS)as
OBJCOPY = $(CROSS)objcopy
OBJDUMP = $(CROSS)objdump

all: $(PRJ).bin
	ls -l *.bin

$(PRJ).bin: $(PRJ).elf
	$(OBJCOPY) -O binary $< $@
	$(OBJDUMP) -d $< > $(PRJ).lst
	./mktiny210spl.exe $(PRJ).bin $(PRJ)-sd.bin

$(PRJ).elf: $(OBJ)
	$(LD) $^ -o $@

b burn:
	sudo dd iflag=dsync oflag=dsync if=$(PRJ)-sd.bin of=/dev/sdb seek=1

clean:
	-rm *.o
	-rm *.elf
	-rm *.bin
	-rm *.lst
	
