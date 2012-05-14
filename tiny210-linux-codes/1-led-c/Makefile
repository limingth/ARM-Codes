all:
	arm-linux-as start.s -o start.o
	arm-linux-gcc -c led.c -o led.o
	arm-linux-ld start.o led.o -o led.elf
	arm-linux-objcopy -O binary led.elf led.bin
	arm-linux-objdump -d led.elf > led.lst
	./mktiny210spl.exe led.bin sd-led.bin
	ls -l *.bin

burn:
	sudo dd iflag=dsync oflag=dsync if=sd-led.bin of=/dev/sdb seek=1

