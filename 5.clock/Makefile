led.bin: start.o clock.o led.o
	arm-linux-ld -Ttext 0x50000000 -o led.elf start.o clock.o led.o
	arm-linux-objcopy -O binary led.elf led.bin
	arm-linux-objdump -D led.elf > led.dis

%.o : %.S
	arm-linux-gcc -o $@ $< -c

%.o : %.c
	arm-linux-gcc -o $@ $< -c
	
clean:
	rm *.o led.elf led.bin led.dis
