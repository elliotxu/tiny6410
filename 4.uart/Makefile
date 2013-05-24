#设置编译器指令变量
CC      = arm-linux-gcc
LD      = arm-linux-ld
AR      = arm-linux-ar
OBJCOPY = arm-linux-objcopy
OBJDUMP = arm-linux-objdump

INCLUDEDIR 	:= $(shell pwd)/include				#包含的路径
CFLAGS 		:= -Wall -Os -fno-builtin-printf 	#c文件编译条件
CPPFLAGS   	:= -nostdinc -I$(INCLUDEDIR)		#c++文件编译条件

#导出变量
export 	CC AR LD OBJCOPY OBJDUMP INCLUDEDIR CFLAGS CPPFLAGS 

objs := start.o main.o uart.o clock.o led.o lib/libc.a

uart.bin: $(objs)
	$(LD) -Ttext 0x50000000 -o uart.elf $^
	$(OBJCOPY) -O binary uart.elf uart.bin
	$(OBJDUMP) -D uart.elf > uart.dis 
	
.PHONY : lib/libc.a
lib/libc.a:
	cd lib; make; cd ..
%.o : %.S
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o  $@ $< 
%.o : %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<
clean:
	make  clean -C lib
	rm *.o uart.elf uart.bin uart.dis