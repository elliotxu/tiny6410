INCLUDEDIR 	:= $(shell pwd)/include
CFLAGS 		:= -Wall -Os -fno-builtin-printf 
CPPFLAGS   	:= -nostdinc -I$(INCLUDEDIR)

uart.bin: start.o main.o uart.o clock.o led.o lib/libc.a
	arm-linux-ld -Ttext 0x50000000 -o uart.elf start.o main.o uart.o clock.o led.o lib/libc.a
	arm-linux-objcopy -O binary uart.elf uart.bin
	arm-linux-objdump -D uart.elf > uart.dis
.PHONY : lib/libc.a
lib/libc.a:
	cd lib; make; cd ..
%.o : %.S
	arm-linux-gcc -o  $@ $< -c
%.o : %.c
	arm-linux-gcc $(CPPFLAGS) $(CFLAGS) -c -o $@ $<
clean:
	rm *.o uart.elf uart.bin uart.dis