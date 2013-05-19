led.bin: start.o
	arm-linux-ld -Ttext 0 -o led.elf start.o		#链接
	arm-linux-objcopy -O binary led.elf led.bin		#产生bin文件
	arm-linux-objdump -D led.elf > led.dis			#反汇编

start.o : start.S
	arm-linux-gcc -o start.o start.S -c

clean:
	rm *.o led.elf led.bin led.dis
