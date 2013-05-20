uart.bin: start.o main.o uart.o clock.o led.o
	arm-linux-ld -Ttext 0 -o uart.elf start.o main.o uart.o clock.o led.o
	arm-linux-objcopy -O binary uart.elf uart.bin
	arm-linux-objdump -D uart.elf > uart.dis
%.o : %.S
	arm-linux-gcc -o $@ $< -c
%.o : %.c
	arm-linux-gcc -o $@ $< -c
clean:
	rm *.o uart.elf uart.bin uart.dis