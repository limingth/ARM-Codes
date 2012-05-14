all:
	arm-linux-as led.s -o led.o
	arm-linux-ld led.o -o led.elf
	arm-linux-objcopy -O binary led.elf led.bin
	arm-linux-objdump -d led.elf > led.lst
	./mktiny210spl.exe led.bin sd-led.bin
	ls -l *.bin
