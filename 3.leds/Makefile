led.bin: start.o led.o
	arm-linux-ld -Ttext 0x50000000 -o led.elf start.o led.o	#链接到0x50000000地址，因为我们下载到DDR中运行，DDR开始地址是0x50000000
	arm-linux-objcopy -O binary led.elf led.bin		#生产bin文件
	arm-linux-objdump -D led.elf > led.dis			#产生反汇编文件

start.o : start.S
	arm-linux-gcc -o start.o start.S -c	#编译start.S
led.o:led.c
	arm-linux-gcc -o led.o led.c -c		#编译led.c
clean:
	rm *.o led.elf led.bin led.dis
